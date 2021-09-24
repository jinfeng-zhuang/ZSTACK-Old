#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>
#include <zstack/mp4.h>

int mp4box_parser_ctts(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 count;
	u8 prefix[4] = { 0, 0, 0, 1 };
	int sample_count;
	int sample_delta;
	u32* data = (u32*)&buffer[4 + 4];
	u32 fullbox_info;
	u32 fullbox_version;
	u32 fullbox_flag;

	fullbox_info = *(u32*)buffer;

	fullbox_version = fullbox_info >> 24;
	fullbox_flag = fullbox_info & 0x00FFFFFF;

	count = BE_read_u32(&buffer[4]);

	info("\n");

	info("Count: %d\n", count);

	for (i = 0; i < count; i++) {
		sample_count = BE_read_u32(&data[i * 2 + 0]);
		sample_delta = BE_read_u32(&data[i * 2 + 1]);

		info("SampleCount = %d, SampleOffset = %d\n", sample_count, sample_delta);
	}

	return 0;
}