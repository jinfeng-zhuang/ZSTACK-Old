#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>

int mp4box_parser_stts(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 len;
	u8 prefix[4] = { 0, 0, 0, 1 };
	int sample_count;
	int sample_delta;
	u32 fullbox_info;
	u32* data = (u32*)&buffer[8];

	fullbox_info = *(u32*)buffer;

	len = BE_read_u32(&buffer[4]);

	info("Decoding Time to Sample Box\n");

	info("Entry = %d\n", len);

	for (i = 0; i < len; i++) {
		sample_count = BE_read_u32(&data[i * 2 + 0]);
		sample_delta = BE_read_u32(&data[i * 2 + 1]);

		info("[%d] Count = %d, Delta = %d (SUM = %d)\n", i, sample_count, sample_delta, sample_count * sample_delta);
	}

	return 0;
}