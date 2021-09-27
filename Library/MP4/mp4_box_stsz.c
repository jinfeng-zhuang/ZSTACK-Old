#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/mp4.h>
#include <zstack/log.h>

int mp4box_parser_stsz(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 default_sample_size;
	u32 sample_count;
	u32 entry_size;
	u32* data = (u32*)&buffer[20];
	struct mp4_box_stsz* stsz;

	info("\n");

	if (NULL == arg)
		goto END;

	stsz = &((struct MP4Track*)arg)->stsz;

	default_sample_size = BE_read_u32(&buffer[12]);
	sample_count = BE_read_u32(&buffer[16]);

	info("Sample Size Box\n");

	info("DefaultSampleSize = %d\n", default_sample_size);
	info("EntryCount = %d\n", sample_count);

	stsz->sample_size = default_sample_size;
	stsz->sample_count = sample_count;
	stsz->entry_size = malloc(sizeof(u32) * stsz->sample_count);

	for (i = 0; i < sample_count; i++) {
		entry_size = BE_read_u32(&data[i]);

		info("[%d] SampleSize = %d\n", i, entry_size);

		if (stsz->entry_size) {
			stsz->entry_size[i] = entry_size;
		}
	}

	info("\n");

END:
	return total;
}