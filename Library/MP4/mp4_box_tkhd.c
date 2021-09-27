#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>
#include <zstack/mp4.h>

int mp4box_parser_tkhd(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 entry_count;
	int first_chunk;
	int sample_per_chunk;
	int sample_desc_index;
	struct MP4Track* track;
	u8 version;

	warn("tkhd total = %d\n", total);

	version = buffer[sizeof(struct Box)];

	if (NULL == arg)
		goto END;

	track = arg;

	if (1 == version) {
		track->id = BE_read_u32(&buffer[offsetof(struct MP4TrackHeaderV1, track_id)]);
		track->durationTrack = BE_read_u64(&buffer[offsetof(struct MP4TrackHeaderV1, duration)]);
		track->width = BE_read_u16(&buffer[offsetof(struct MP4TrackHeaderV1, width)]);
		track->height = BE_read_u16(&buffer[offsetof(struct MP4TrackHeaderV1, height)]);
		track->volumn = BE_read_u8(&buffer[offsetof(struct MP4TrackHeaderV1, volumn)]);
	}
	else {
		track->id = BE_read_u32(&buffer[offsetof(struct MP4TrackHeader, track_id)]);
		track->durationTrack = BE_read_u32(&buffer[offsetof(struct MP4TrackHeader, duration)]);
		track->width = BE_read_u16(&buffer[offsetof(struct MP4TrackHeader, width)]);
		track->height = BE_read_u16(&buffer[offsetof(struct MP4TrackHeader, height)]);
		track->volumn = BE_read_u8(&buffer[offsetof(struct MP4TrackHeader, volumn)]);
	}

	warn("version = %d, duration = %d, resolution = %dx%d, volumn = %d\n",
		version, track->durationTrack, track->width, track->height, track->volumn);

	info("\n");

END:
	return total;
}
