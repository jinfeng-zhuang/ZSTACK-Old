#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>

int mp4box_parser_stsc(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 len;
	u8 prefix[4] = { 0, 0, 0, 1 };
	int first_chunk;
	int sample_per_chunk;
	int sample_desc_index;
	u32 fullbox_info;
	u32* data = (u32*)&buffer[8];

	info("\n");

	fullbox_info = *(u32*)buffer;

	len = BE_read_u32(&buffer[4]);

	info("Sample-To-Chunk Atom\n");

	info("EntryCount = %d\n", len);

	for (i = 0; i < len; i++) {
		first_chunk = BE_read_u32(&data[i * 3 + 0]);
		sample_per_chunk = BE_read_u32(&data[i * 3 + 1]);
		sample_desc_index = BE_read_u32(&data[i * 3 + 2]);

		info("[%d] first_chunk = %d, sample_per_chunk = %d, sample_desc_index = %d\n",
			i, first_chunk, sample_per_chunk, sample_desc_index);
	}

	info("\n");

	return 0;
}