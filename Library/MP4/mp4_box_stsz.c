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
	int sum = 0;

	info("\n");

	default_sample_size = BE_read_u32(&buffer[12]);
	sample_count = BE_read_u32(&buffer[16]);

	info("Sample Size Box\n");

	info("DefaultSampleSize = %d\n", default_sample_size);
	info("EntryCount = %d\n", sample_count);

	//mp4.stsz_count = sample_count;
	//mp4.stsz_offset = malloc(mp4.stsz_count * sizeof(int));

	for (i = 0; i < sample_count; i++) {
		entry_size = BE_read_u32(&data[i]);

		//if (mp4.stsz_offset) {
		//	mp4.stsz_offset[i] = entry_size;
		//}

		info("[%d] SampleSize = %d\n", i, entry_size);

		sum += entry_size;

		if (734 == sample_count) {
			warn("[%d] sum = %d\n", i, sum);
		}
	}

	info("\n");

	return total;
}