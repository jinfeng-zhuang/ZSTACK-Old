#include <zstack/zstack.h>
#include <zstack/mp4.h>

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

static struct mp4box_parser parser[] = {
	//{"ftyp", mp4box_parser_ftyp},
	{"mdat", mp4box_parser_mdat}, // data
	{"stsd", mp4box_parser_stsd}, // PPS/SPS
	//{"stts", mp4box_parser_stts}, // time 2 sample
	//{"ctts", mp4box_parser_ctts}, // time 2 sample
	//{"stss", mp4box_parser_stss}, // sync sample
	//{"stsc", mp4box_parser_stsc}, // sample 2 chunk
	//{"stco", mp4box_parser_stco}, // chunk offset
	//{"stsz", mp4box_parser_stsz}, // sample size
	{"avc1", mp4box_parser_avc1}, // sample size
	{"avcC", mp4box_parser_avcC}, // sample size
	{"meta", mp4box_parser_meta}, // sample size
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

void mp4_box_parse(const char *type, u8 *buffer, u32 length)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(parser); i++) {
		if (0 == strcmp(parser[i].type, type)) {
			parser[i].func(buffer, length, &mp4info);
		}
	}
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

RESULT mp4_box_scan(int depth, u8* buffer, u32 len)
{
	int i;
	struct MP4Box *box;
	u32 size = 0;
	u8 type[5];
	u64 large_size;
	struct mp4box_parser* parser;
	int header_size = sizeof(struct Box);
	u8 prefix[] = { 0,0,0,1 };
	u8 prefix2[] = { 0,0,1 };

	if ((NULL == buffer) || (len < 8)) {
		//warn("param not valid\n");
		return -1;
	}

	for (i = 0; i < len - 8; i += size) {
		size = BE_read_u32(&buffer[i]);
		memcpy(type, &buffer[i + 4], 4);
		type[4] = '\0';

		if (1 == size) {
			//print_depth(depth);
			//warn("large size not support: %x\n", size);
			return -1;
		}
		else if (!mp4_is_type_valid(type)) {
			return -1;
		}
		else {
			print_depth(depth);
			warn("%s\n", type);
		}

		parser = find_parser(type);
		if (parser) {
			if (parser->func) {
				header_size = parser->func(&buffer[i], size, &mp4info);
			}
		}

		mp4_box_scan(depth + 1, &buffer[i + header_size], size - header_size);
	}

	if (0 == depth) {
		file_save("output.264", prefix, sizeof(prefix));
		file_append("output.264", mp4info.sps[0], mp4info.sps_len[0]);
		file_append("output.264", prefix, sizeof(prefix));
		file_append("output.264", mp4info.pps[0], mp4info.pps_len[0]);

		for (i = 0; i < mp4info.sample_count; i++) {
			file_append("output.264", prefix2, sizeof(prefix2));
			file_append("output.264", mp4info.sample[i], mp4info.sample_len[i]);
		}
	}

	return 0;
}
