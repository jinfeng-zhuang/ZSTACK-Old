#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>
#include <zstack/mp4.h>

int mp4box_parser_meta(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 count;
	u8 prefix[4] = { 0, 0, 0, 1 };
	int sample_count;
	int sample_delta;
	u32* data = (u32*)&buffer[4 + 4];
	u32 fullbox_info;
	u32 fullbox_version;
	u32 fullbox_flag;

	//hexdump(buffer, total, HEXDUMP_ASCII);

	return sizeof(struct FullBox);
}
