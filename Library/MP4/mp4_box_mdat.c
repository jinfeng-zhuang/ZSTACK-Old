#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/mp4.h>
#include <zstack/zstack.h>

int mp4box_parser_mdat(u8* buffer, u32 total, void *arg) {
	u8 prefix[] = { 0, 0, 1 };
	struct MP4Info* mp4info = arg;
	int i;
	int offset;
	int sample_len;
	int sample_count = 0;

	for (offset = 8; offset < total; offset += sample_len + 4) {
		sample_len = BE_read_u32(&buffer[offset]);
		sample_count++;
	}

	mp4info->sample_count = sample_count;
	mp4info->sample_len = malloc(sizeof(u32) * mp4info->sample_count);
	if (NULL == mp4info->sample_len)
		goto END;

	mp4info->sample = malloc(sizeof(u8*) * mp4info->sample_count);
	if (NULL == mp4info->sample)
		goto END;

	for (i = 0, offset = 8; offset < total, i < mp4info->sample_count; offset += mp4info->sample_len[i] + 4, i++) {
		mp4info->sample_len[i] = BE_read_u32(&buffer[offset]);

		//warn("offset = %x mp4info->sample_len[i] = %x\n", offset, mp4info->sample_len[i]);
		
		mp4info->sample[i] = malloc(mp4info->sample_len[i]);
		if (NULL == mp4info->sample[i])
			goto END;
		
		memcpy(mp4info->sample[i], &buffer[offset + 4], mp4info->sample_len[i]);
	}

END:
	return sizeof(struct Box);
}
