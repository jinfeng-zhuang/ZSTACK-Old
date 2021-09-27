#include <zstack/zstack.h>
#include <zstack/mp4.h>
#include <zstack/adts.h>

static struct MP4Info mp4info;

extern int mp4box_parser_ftyp(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_mdat(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_stsd(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_stts(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_stss(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_stsc(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_stco(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_stsz(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_ctts(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_avc1(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_avcC(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_meta(u8* buffer, u32 total, void* arg);
extern int mp4box_parser_tkhd(u8* buffer, u32 total, void* arg);

static struct mp4box_parser parser[] = {
	//{"ftyp", mp4box_parser_ftyp},
	{"mdat", mp4box_parser_mdat}, // data
	{"stsd", mp4box_parser_stsd}, // PPS/SPS
	//{"stts", mp4box_parser_stts}, // time 2 sample
	//{"ctts", mp4box_parser_ctts}, // time 2 sample
	//{"stss", mp4box_parser_stss}, // sync sample
	{"stsc", mp4box_parser_stsc}, // sample 2 chunk
	{"stco", mp4box_parser_stco}, // chunk offset
	{"stsz", mp4box_parser_stsz}, // sample size
	{"avc1", mp4box_parser_avc1}, // sample size
	{"avcC", mp4box_parser_avcC}, // sample size
	{"meta", mp4box_parser_meta}, // sample size
	{"tkhd", mp4box_parser_tkhd}, // sample size
};

static struct mp4box_parser* find_parser(const char* type)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(parser); i++) {
		if (0 == strcmp(parser[i].type, type)) {
			return &parser[i];
		}
	}

	return NULL;
}

int mp4_is_type_valid(const char* type)
{
	int i;

	for (i = 0; i < 4; i++) {
		if (! is_printable(type[i])) {
			return FALSE;
		}
	}

	return TRUE;
}

static void print_depth(int depth)
{
	int j;

	for (j = 0; j < depth; j++) {
		warn("..");
	}
}

struct mp4sample {
	u32 count;
	u32* offset;
	u32* size;
};

struct MP4Track gTrack;

static int find_offset(struct MP4Track* track, int sample, u32 *start)
{
	u32 i;
	u32 j;
	u32 sum = 0;

	if (NULL == track->stsc.first_chunk)
		return -1;

	for (i = 0; i < track->stsc.entry_count - 1; i++) {
		for (j = track->stsc.first_chunk[i]; j < track->stsc.first_chunk[i + 1]; j++) {
			sum += track->stsc.sample_per_chunk[i];

			if (sample < sum) {
				*start = sum - track->stsc.sample_per_chunk[i];
				return j - 1; // as start from '1', but stco start from '0'
			}
		}
	}

	if (i == track->stsc.entry_count - 1) {
		sum += track->stsc.sample_per_chunk[i];

		if (sample < sum) {
			*start = sum - track->stsc.sample_per_chunk[i];
			return track->stsc.first_chunk[i];
		}
	}

	return -1;
}

struct putbitctx {
	u8* buffer;
	u32 len;
	u32 pos;
};

void putbit_init(struct putbitctx *pc, u8* buffer, u32 len)
{
	pc->buffer = buffer;
	pc->len = len;
	pc->pos = 0;
}

void put_bits(struct putbitctx* pc, u32 count, u32 data)
{
	u32 bytepos;
	u32 bitpos;
	u32 i;
	u8 bit;

	for (i = 0; i < count; i++) {
		bytepos = pc->pos / 8;
		bitpos = pc->pos % 8;

		bit = data >> (count - 1 - i) & 0x1;

		pc->buffer[bytepos] = pc->buffer[bytepos] | (bit ? (1 << (7 - bitpos)) : 0);

		pc->pos++;
	}
}

static char filename[FILENAME_MAX];
static u8 frame[1 << 20];

static void track_dump(struct MP4Track* track)
{
	struct mp4sample sample;
	u32 i;
	u32 j;
	u32 chunk;
	u32 start;
	u32 sum;
	
	u8 prefix[] = { 0,0,0,1 };
	u8 prefix2[] = { 0,0,1 };
	u32 frame_size_max = 0;

	sample.count = track->stsz.sample_count;
	sample.offset = malloc(sizeof(u32) * sample.count);
	sample.size = malloc(sizeof(u32) * sample.count);

	if ((NULL == sample.offset) || (NULL == sample.size))
		return;

	for (i = 0; i < sample.count; i++) {
		chunk = find_offset(&gTrack, i, &start);

		//warn("find sample %d  in chunk %d, and base is %d\n", i, chunk+1, start);

		sum = 0;
		for (j = start; j < i; j++) {
			sum += track->stsz.entry_size[j];
		}

		sample.size[i] = track->stsz.entry_size[i];
		sample.offset[i] = track->stco.chunk_offset[chunk] + sum;

		if (sample.size[i] > frame_size_max)
			frame_size_max = sample.size[i];
	}

	for (i = 0; i < 13; i++) {
		//warn("dump: offset = %d size = %d\n", sample.offset[i], sample.size[i]);
	}

	memset(filename, 0, FILENAME_MAX);

	snprintf(filename, FILENAME_MAX, "track-%d.bin", track->id);
	const char* input = "test.mp4";

	warn("DUMP track %d\n", track->id);

	// TODO: HintTrack
	if (0 == track->volumn) {
		

		file_save(filename, prefix, sizeof(prefix));
		file_append(filename, track->stsd.sps[0], track->stsd.sps_len[0]);
		file_append(filename, prefix, sizeof(prefix));
		file_append(filename, track->stsd.pps[0], track->stsd.pps_len[0]);

		// should move to app layer
		for (i = 0; i < sample.count; i++) {
			file_append(filename, prefix2, sizeof(prefix2));
			file_load(input, sample.offset[i] + 4, frame, sample.size[i]); // len + data
			file_append(filename, frame, sample.size[i]);
		}
	}
	else {
		// volumn
		u8 tmpbuf[7];

		struct putbitctx pb;

		for (i = 0; i < sample.count; i++) {

			memset(tmpbuf, 0, 7);

			putbit_init(&pb, tmpbuf, 7);

			/* adts_fixed_header */
			put_bits(&pb, 12, 0xfff);   /* syncword */
			put_bits(&pb, 1, 0);        /* ID */
			put_bits(&pb, 2, 0);        /* layer */
			put_bits(&pb, 1, 1);        /* protection_absent */
			put_bits(&pb, 2, 1); /* profile_objecttype */
			put_bits(&pb, 4, 4);
			put_bits(&pb, 1, 0);        /* private_bit */
			put_bits(&pb, 3, 1); /* channel_configuration */
			put_bits(&pb, 1, 0);        /* original_copy */
			put_bits(&pb, 1, 0);        /* home */

			/* adts_variable_header */
			put_bits(&pb, 1, 0);        /* copyright_identification_bit */
			put_bits(&pb, 1, 0);        /* copyright_identification_start */
			put_bits(&pb, 13, 7 + sample.size[i]); /* aac_frame_length */
			put_bits(&pb, 11, 0x7ff);   /* adts_buffer_fullness */
			put_bits(&pb, 2, 0);        /* number_of_raw_data_blocks_in_frame */

			file_append(filename, tmpbuf, 7);
			file_load(input, sample.offset[i], frame, sample.size[i]);
			file_append(filename, frame, sample.size[i]);
		}
	}
}

RESULT mp4_box_scan(int depth, u8* buffer, u32 len)
{
	int i;
	struct MP4Box *box;
	u32 size = 0;
	u8 type[5];
	u64 large_size;
	struct mp4box_parser* parser;
	int skip_size = sizeof(struct Box);
	u8 prefix[] = { 0,0,0,1 };
	u8 prefix2[] = { 0,0,1 };

	if ((NULL == buffer) || (len < 8)) {
		//warn("param not valid: buffer = %x len = %d\n", buffer, len);
		return -1;
	}

	//warn("%s\n", __func__);

	for (i = 0; i < len - 8; i += size) {
		//hexdump(&buffer[i], 16, HEXDUMP_ASCII);

		size = BE_read_u32(&buffer[i]);
		memcpy(type, &buffer[i + 4], 4);
		type[4] = '\0';

		if (1 == size) {
			//print_depth(depth);
			//warn("large size not support: %x @ %d, %d\n", size, i, len - i);
			//hexdump(&buffer[i], 64, HEXDUMP_ASCII);
			return -1;
		}
		else if (0 == size) {
			return -1;
		}
		else if (size > len) {
			//warn("size not correct: size = %d, len = %d\n", size, len);
			return -1;
		}
		else if (!mp4_is_type_valid(type)) {
			return -1;
		}
		else {
			print_depth(depth);
			warn("%s\n", type);
		}

		if (0 == memcmp(type, "trak", 4)) {
			// dump the complete track raw data here
			if (gTrack.id != 0) {
				track_dump(&gTrack);
			}
		}

		parser = find_parser(type);
		if (parser) {
			if (parser->func) {
				skip_size = parser->func(&buffer[i], size, &gTrack); // input should be track
			}
		}
		else {
			skip_size = sizeof(struct Box);
		}

		mp4_box_scan(depth + 1, &buffer[i + skip_size], size - skip_size);
	}

#if 1
	warn("depth = %d\n", depth);
	if (1 == depth) {
		if (gTrack.id != 0) {
			track_dump(&gTrack);
		}
	}
#endif

	return 0;
}
