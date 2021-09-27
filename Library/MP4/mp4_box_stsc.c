#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>
#include <zstack/mp4.h>

int mp4box_parser_stsc(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 entry_count;
	int first_chunk;
	int sample_per_chunk;
	int sample_desc_index;
	u32* data = (u32*)&buffer[sizeof(struct FullBox) + sizeof(u32)];
	struct mp4_box_stsc* stsc;

	if (NULL == arg)
		goto END;

	stsc = &((struct MP4Track*)arg)->stsc;

	info("\n");

	entry_count = BE_read_u32(&buffer[sizeof(struct FullBox)]);

	stsc->entry_count = entry_count;
	stsc->first_chunk = malloc(sizeof(u32) * stsc->entry_count);
	stsc->sample_per_chunk = malloc(sizeof(u32) * stsc->entry_count);
	stsc->sample_desc_index = malloc(sizeof(u32) * stsc->entry_count);

	info("Sample-To-Chunk Atom\n");

	info("EntryCount = %d\n", entry_count);

	for (i = 0; i < entry_count; i++) {
		first_chunk = BE_read_u32(&data[i * 3 + 0]);
		sample_per_chunk = BE_read_u32(&data[i * 3 + 1]);
		sample_desc_index = BE_read_u32(&data[i * 3 + 2]);

		info("[%d] first_chunk = %d, sample_per_chunk = %d, sample_desc_index = %d\n",
			i, first_chunk, sample_per_chunk, sample_desc_index);

		if (stsc->first_chunk)
			stsc->first_chunk[i] = first_chunk;
		if (stsc->sample_per_chunk)
			stsc->sample_per_chunk[i] = sample_per_chunk;
		if (stsc->sample_desc_index)
			stsc->sample_desc_index[i] = sample_desc_index;
	}

	info("\n");

END:
	return total;
}