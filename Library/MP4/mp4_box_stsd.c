#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/mp4.h>
#include <zstack/log.h>

int mp4box_parser_stsd(u8* buffer, u32 total, void* arg) {
	u32 i;
	u32 len;
	struct MP4Info *info;
	struct SampleDescriptionBox* SDB;
	u32 offset;

	u8 prefix[4] = { 0, 0, 0, 1 };

	info = (struct MP4Info*)arg;

	SDB = (struct SampleDescriptionBox*)&info->stsd.SampleDescBox;

	//SDB->entry_count = BE_read_u32(&buffer[0]);

#if 0
	// BOX
	SDB->AVC.VSE.SE.box.size = BE_read_u32(&buffer[4]);
	SDB->AVC.VSE.SE.box.type = BE_read_u32(&buffer[8]);
	memcpy(SDB->AVC.VSE.SE.rsvd, &buffer[12], 6);
	SDB->AVC.VSE.SE.data_reference_index = BE_read_u16(&buffer[18]);
	SDB->AVC.VSE.pre_defined = BE_read_u16(&buffer[22]);
	SDB->AVC.VSE.rsvd = BE_read_u16(&buffer[26]);
	SDB->AVC.VSE.pre_defined2[0] = BE_read_u32(&buffer[28]);
	SDB->AVC.VSE.pre_defined2[1] = BE_read_u32(&buffer[32]);
	SDB->AVC.VSE.pre_defined2[2] = BE_read_u32(&buffer[36]);
	SDB->AVC.VSE.width = BE_read_u16(&buffer[40]);
	SDB->AVC.VSE.height = BE_read_u16(&buffer[42]);
	SDB->AVC.VSE.horiz = BE_read_u32(&buffer[44]);
	SDB->AVC.VSE.vert = BE_read_u32(&buffer[48]);
	SDB->AVC.VSE.rsvd2 = BE_read_u32(&buffer[52]);
	SDB->AVC.VSE.frame_count = BE_read_u16(&buffer[56]);
	memcpy(SDB->AVC.VSE.compressorname, &buffer[58], 32);
	SDB->AVC.VSE.depth = BE_read_u16(&buffer[90]);
	SDB->AVC.VSE.pre_defined3 = BE_read_u16(&buffer[92]);

	info("width = %d height = %d, horiz = %x vert = %x, frame count = %d, name = %s, depth = %d\n",
		SDB->AVC.VSE.width, SDB->AVC.VSE.height, SDB->AVC.VSE.horiz, SDB->AVC.VSE.vert,
		SDB->AVC.VSE.frame_count,
		SDB->AVC.VSE.compressorname, SDB->AVC.VSE.depth);
#endif

	// Box
#if 0
	SDB->AVC.avc.box.size = BE_read_u32(&buffer[98]);
	SDB->AVC.avc.box.type = BE_read_u32(&buffer[102]);
	SDB->AVC.avc.AVCConfig.configurationVersion = BE_read_u8(&buffer[102]);
	SDB->AVC.avc.AVCConfig.AVCProfileIndication = BE_read_u8(&buffer[103]);
	SDB->AVC.avc.AVCConfig.profile_compatibility = BE_read_u8(&buffer[104]);
	SDB->AVC.avc.AVCConfig.AVCLevelIndication = BE_read_u8(&buffer[105]);
	SDB->AVC.avc.AVCConfig.reserved_6bits_111111 = BE_read_u6(&buffer[106]);
	SDB->AVC.avc.AVCConfig.lengthSizeMinusOne = buffer[106] & 0x3;
	SDB->AVC.avc.AVCConfig.reserved_3bits_111 = buffer[107] >> 5;
	SDB->AVC.avc.AVCConfig.numOfSequenceParameterSets = buffer[107] & 0x1F;

	SDB->AVC.avc.AVCConfig.sequenceParameterSetLength =
		malloc(sizeof(u16) * SDB->AVC.avc.AVCConfig.numOfSequenceParameterSets);
	SDB->AVC.avc.AVCConfig.sequenceParameterSetNALUnit =
		malloc(sizeof(u8*) * SDB->AVC.avc.AVCConfig.numOfSequenceParameterSets);


	offset = 108;
	for (i = 0; i < SDB->AVC.avc.AVCConfig.numOfSequenceParameterSets; i++) {
		SDB->AVC.avc.AVCConfig.sequenceParameterSetLength[i] = BE_read_u16(&buffer[offset]);
		SDB->AVC.avc.AVCConfig.sequenceParameterSetNALUnit[i] =
			malloc(sizeof(u8) * SDB->AVC.avc.AVCConfig.sequenceParameterSetLength[i]);
		memcpy(SDB->AVC.avc.AVCConfig.sequenceParameterSetNALUnit[i],
			&buffer[110 + ]);

		offset += 2;
		offset += SDB->AVC.avc.AVCConfig.sequenceParameterSetLength[i];
	}
#endif

	return 8+4+4;
}