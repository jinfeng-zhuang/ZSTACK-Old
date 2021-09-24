#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>

int mp4box_parser_stco(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 len;
	u8 prefix[4] = { 0, 0, 0, 1 };
	unsigned int offset;
	u32 fullbox_info;
	u32* data = (u32*)&buffer[8];

	info("\n");

	fullbox_info = *(u32*)buffer;

	len = BE_read_u32(&buffer[4]);

	info("Chunk Offset in 'mdat' box.\n");

	info("EntryCount = %d\n", len);

	for (i = 0; i < len; i++) {
		offset = BE_read_u32(&data[i]);

		info("[%d] Offset = %d\n", i, offset);
	}

	info("\n");

	return 0;
}