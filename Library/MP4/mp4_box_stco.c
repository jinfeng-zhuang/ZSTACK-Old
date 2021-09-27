#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>
#include <zstack/mp4.h>

int mp4box_parser_stco(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 entry_count;
	unsigned int offset;
	u32* data = (u32*)&buffer[sizeof(struct FullBox) + sizeof(u32)];
	struct mp4_box_stco* stco;

	if (NULL == arg)
		goto END;

	stco = &((struct MP4Track*)arg)->stco;

	info("\n");

	entry_count = BE_read_u32(&buffer[sizeof(struct FullBox)]);

	stco->entry_count = entry_count;
	stco->chunk_offset = malloc(sizeof(u32) * stco->entry_count);

	info("Chunk Offset in 'mdat' box.\n");

	info("EntryCount = %d\n", entry_count);

	for (i = 0; i < entry_count; i++) {
		offset = BE_read_u32(&data[i]);

		info("[%d] Offset = %d\n", i, offset);

		if (stco->chunk_offset)
			stco->chunk_offset[i] = offset;
	}

	info("\n");

END:
	return total;
}