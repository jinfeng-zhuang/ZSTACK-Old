#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>

int mp4box_parser_stss(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 len;
	u8 prefix[4] = { 0, 0, 0, 1 };
	int sample_number;
	u32 fullbox_info;
	u32* data = (u32*)&buffer[8];

	info("\n");

	fullbox_info = *(u32*)buffer;

	len = BE_read_u32(&buffer[4]);

	info("Sync Sample Atom\n");

	info("Entry = %d\n", len);

	for (i = 0; i < len; i++) {
		sample_number = BE_read_u32(&data[i]);

		info("[%d] sample_number = %d\n", i, sample_number);
	}

	info("\n");

	return 0;
}