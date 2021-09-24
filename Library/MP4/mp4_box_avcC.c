#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>
#include <zstack/mp4.h>

int mp4box_parser_avcC(u8* buffer, u32 total, void* arg) {
	struct AVCDecoderConfigurationRecord config;
	u8* data = &buffer[8];
	int i;
	int offset;
	u8 prefix[] = { 0, 0, 0, 1 };
	struct MP4Info* mp4info = arg;

	memset(&config, 0, sizeof(struct AVCDecoderConfigurationRecord));

	hexdump(buffer, total, HEXDUMP_ASCII);

	offset = 0;
	config.configurationVersion = BE_read_u8(&data[0]);
	config.AVCProfileIndication = BE_read_u8(&data[1]);
	config.profile_compatibility = BE_read_u8(&data[2]);
	config.AVCLevelIndication = BE_read_u8(&data[3]);
	config.reserved_6bits_111111 = data[4] >> 2;
	config.lengthSizeMinusOne = data[4] & 0x3;
	config.reserved_3bits_111 = data[5] >> 5;

	//-------------
	// SPS
	//-------------
	config.numOfSequenceParameterSets = data[5] & 0x1F;

	config.sequenceParameterSetLength = malloc(sizeof(u16) * config.numOfSequenceParameterSets);
	if (NULL == config.sequenceParameterSetLength) {
		warn("malloc failed\n");
		goto END;
	}
	config.sequenceParameterSetNALUnit = malloc(sizeof(u8*) * config.numOfSequenceParameterSets);
	if (NULL == config.sequenceParameterSetNALUnit) {
		warn("malloc failed\n");
		goto END;
	}

	mp4info->sps_count = config.numOfSequenceParameterSets;
	mp4info->sps_len = config.sequenceParameterSetLength;
	mp4info->sps = config.sequenceParameterSetNALUnit;

	warn("SPS count = %d\n", config.numOfSequenceParameterSets);
	
	offset = 6;
	for (i = 0; i < config.numOfSequenceParameterSets; i++) {
		config.sequenceParameterSetLength[i] = BE_read_u16(&data[offset]);
		offset += sizeof(u16);

		warn("SPS len = %d\n", config.sequenceParameterSetLength[i]);

		config.sequenceParameterSetNALUnit[i] = malloc(config.sequenceParameterSetLength[i]);
		if (NULL == config.sequenceParameterSetNALUnit[i]) {
			warn("malloc failed\n");
			goto END;
		}

		memcpy(config.sequenceParameterSetNALUnit[i], &data[offset], config.sequenceParameterSetLength[i]);
		offset += config.sequenceParameterSetLength[i];

		hexdump(config.sequenceParameterSetNALUnit[i], config.sequenceParameterSetLength[i], 0);

		//file_append("output.264", prefix, 4);
		//file_append("output.264", config.sequenceParameterSetNALUnit[i], config.sequenceParameterSetLength[i]);
	}

	//-------------
	// PPS
	//-------------
	config.numOfPictureParameterSets = data[offset];

	config.pictureParameterSetLength = malloc(sizeof(u16) * config.numOfPictureParameterSets);
	if (NULL == config.pictureParameterSetLength) {
		warn("malloc failed\n");
		goto END;
	}
	config.pictureParameterSetNALUnit = malloc(sizeof(u8*) * config.numOfPictureParameterSets);
	if (NULL == config.pictureParameterSetNALUnit) {
		warn("malloc failed\n");
		goto END;
	}

	mp4info->pps_count = config.numOfPictureParameterSets;
	mp4info->pps_len = config.pictureParameterSetLength;
	mp4info->pps = config.pictureParameterSetNALUnit;

	warn("PPS count = %d\n", config.numOfPictureParameterSets);

	offset += 1;
	for (i = 0; i < config.numOfPictureParameterSets; i++) {
		config.pictureParameterSetLength[i] = BE_read_u16(&data[offset]);
		offset += sizeof(u16);

		warn("PPS len = %d\n", config.pictureParameterSetLength[i]);

		config.pictureParameterSetNALUnit[i] = malloc(config.pictureParameterSetLength[i]);
		if (NULL == config.pictureParameterSetNALUnit[i]) {
			warn("malloc failed\n");
			goto END;
		}

		memcpy(config.pictureParameterSetNALUnit[i], &data[offset], config.pictureParameterSetLength[i]);
		offset += config.pictureParameterSetLength[i];

		hexdump(config.pictureParameterSetNALUnit[i], config.pictureParameterSetLength[i], 0);

		//file_append("output.264", prefix, 4);
		//file_append("output.264", config.pictureParameterSetNALUnit[i], config.pictureParameterSetLength[i]);
	}

	config.reserved_6bits_111111_2 = data[offset] >> 2;
	config.chroma_format = data[offset++] & 0x3;
	config.reserved_5bits_11111 = data[offset] >> 3;
	config.bit_depth_luma_minus8 = data[offset++] & 0x7;
	config.reserved_5bits_11111_2 = data[offset] >> 3;
	config.bit_depth_chroma_minus8 = data[offset++] & 0x7;
	config.numOfSequenceParameterSetExt = data[offset++];

	warn("ChromaFormat = %d\n", config.chroma_format);

	warn("numOfSequenceParameterSetExt = %d\n", config.numOfSequenceParameterSetExt);

END:
	return sizeof(struct Box);
}
