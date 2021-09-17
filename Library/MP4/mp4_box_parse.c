#include <zstack/zstack.h>

enum {
	MP4BOX_FTYP,
	MP4BOX_MDAT,
	MP4BOX_STSD,
};

typedef int (*mp4box_parser_func)(u8* buffer, u32 len);

u32 Endian_B2L_U32(u32 value) {
	return ((value >> 24) & 0xFF)
		| (((value >> 16) & 0xFF) << 8)
		| (((value >> 8) & 0xFF) << 16)
		| (((value >> 0) & 0xFF) << 24);
}

int mp4box_parser_ftyp(u8* buffer, u32 total) {
	return 0;
}

int mp4box_parser_mdat(u8* buffer, u32 total) {
	u32 i;
	u32 len;
	u8 prefix[4] = { 0, 0, 0, 1 };

	for (i = 0; i < total - 4; i+=len+4) {
		len = *((u32*)&buffer[i]);
		len = Endian_B2L_U32(len);
		file_append("raw.bin", prefix, 4);
		file_append("raw.bin", &buffer[i + 4], len);
	}

	return 0;
}

struct mp4box_parser {
	char type[5];
	mp4box_parser_func func;
};

struct mp4box_stsd {
	u8 version[4];
	u32 entry_count;
	u32 size;
	u8 type[4];
	u8 rsvd[6];
	u16 ref_idx;
	u16 pre_def;
	u16 rsvd2;
	u8 pre_def2[12];
	u16 width;
	u16 height;
	u32 dpi_h;
	u32 dpi_v;
	u32 rsvd3;
	u16 frame_count;
	u8 compress_name[32];
	u16 depth;
	u16 pre_def3;
	u32 len;
	u8 type2[4];
	u8 conf;
	u8 profile;
	u8 compa;
	u8 level;
	u8 rsvd4;
	u8 sps_count;
	u16 sps_len;
	u8 sps[1];
	// u8 pps_count
	// u16 pps_len;
	// u8 pps[1]
};

int mp4box_parser_stsd(u8* buffer, u32 total) {
	u32 i;
	u32 len;
	u8 prefix[4] = { 0, 0, 0, 1 };

	for (i = 0; i < total - 4; i += len + 4) {
		len = *((u32*)&buffer[i]);
		len = Endian_B2L_U32(len);

		file_append("sps.bin", prefix, 4);
		file_append("sps.bin", &buffer[i + 4], len);
	}

	return 0;
}

int mp4box_parser_stts(u8* buffer, u32 total) {
	u32 i;
	u32 len;
	u8 prefix[4] = { 0, 0, 0, 1 };
	int sample_count;
	int sample_delta;
	u32* data = (u32 *)&buffer[4];

	len = *((u32*)&buffer[0]);

	warn("STTS: %d\n", len);

	for (i = 0; i < len; i++) {
		sample_count = data[i * 2 + 0];
		sample_delta = data[i * 2 + 1];
		
		warn("%d - %d\n", sample_count, sample_delta);
	}

	return 0;
}

int mp4box_parser_ctts(u8* buffer, u32 total) {
	u32 i;
	u32 count;
	u8 prefix[4] = { 0, 0, 0, 1 };
	int sample_count;
	int sample_delta;
	u32* data = (u32*)&buffer[4+4];
	u32 fullbox_info;
	u32 fullbox_version;
	u32 fullbox_flag;

	fullbox_info = *(u32*)buffer;

	fullbox_version = fullbox_info >> 24;
	fullbox_flag = fullbox_info & 0x00FFFFFF;

	count = *(u32*)(&buffer[4]);
	count = Endian_B2L_U32(count);

	warn("CTTS: %d\n", count);

#if 1
	for (i = 0; i < count; i++) {
		sample_count = Endian_B2L_U32(data[i * 2 + 0]);
		sample_delta = Endian_B2L_U32(data[i * 2 + 1]);

		warn("%d - %d\n", sample_count, sample_delta);
	}
#endif

	return 0;
}

static struct mp4box_parser parser[] = {
	{"ftyp", mp4box_parser_ftyp},
	{"mdat", mp4box_parser_mdat},
	{"stsd", mp4box_parser_stsd},
	{"stts", mp4box_parser_stts},
	{"ctts", mp4box_parser_ctts},
};

void mp4_box_parse(const char *type, u8 *buffer, u32 length)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(parser); i++) {
		if (0 == strcmp(parser[i].type, type)) {
			parser[i].func(buffer, length);
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

RESULT mp4_box_scan(int depth, u8* buffer, u32 len)
{
	int i;
	struct MP4Box *box;
	u32 size = 0;
	u8 type[5];
	u64 large_size;

	if ((NULL == buffer) || (len < 8)) {
		//warn("param not valid\n");
		return -1;
	}

	for (i = 0; i < depth; i++) {
		warn("  ");
	}

	for (i = 0; i < len - 8; i += size) {
		box = &buffer[i];

		size = Endian_B2L_U32(box->size);
		memcpy(type, &buffer[i + 4], 4);
		type[4] = '\0';

		if (1 == size) {
			//warn("large size not support\n");
			return -1;
		}

		if (!mp4_is_type_valid(type)) {
			//warn("type [%s] not valid\n", type);
			return -1;
		}

		mp4_box_parse(type, &buffer[i + 8], size - 8);

		mp4_box_scan(depth + 1, &buffer[i + 8], size - 8);
	}

	return 0;
}
