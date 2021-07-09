#include <zstack/codec.h>
#include <zstack/log.h>
#include <zstack/misc.h>
#include "sps.h"
#include "codec.h"
#include "nalu.h"

struct seq_parameter_set_rbsp sps;

struct seq_parameter_set_rbsp* sps_get(unsigned int id)
{
	return &sps;
}

int decode_one_frame(unsigned char* buffer, unsigned int length)
{
	u32 offset;
	u32 size;
	u8* nalu_payload;
	u32 naly_payload_size;
	u8 nalu_type;

	offset = 0;
	size = 0;

	while (nalu_get(buffer, length, &offset, &size))
	{
		nalu_payload = h264_nalu_parse(&buffer[offset], length, &naly_payload_size, &nalu_type);
		if (nalu_payload) {
			switch (nalu_type) {
			case NALU_TYPE_SPS:
				sps_parse(&sps, nalu_payload, naly_payload_size);
				sps_dump(&sps);
				break;
			case NALU_TYPE_PPS:
				break;
			default:
				break;
			}
		}

		DEBUG("press enter to parse next\n");
		(void)getchar();
	}

	return 0;
}