#include <zstack/codec.h>
#include <zstack/log.h>
#include <zstack/misc.h>
#include <zstack/types.h>
#include "codec.h"
#include "sps.h"
#include "pps.h"
#include "nalu.h"

struct seq_parameter_set_rbsp sps;
struct pic_parameter_set_rbsp pps;
struct nalu nalu;

struct seq_parameter_set_rbsp* sps_get(unsigned int id)
{
	return &sps;
}

struct pic_parameter_set_rbsp* pps_get(unsigned int id)
{
	return &pps;
}

struct nalu* nalu_get_info(void)
{
	return &nalu;
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
				pps_parse(&pps, nalu_payload, naly_payload_size);
				pps_dump(&pps);
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