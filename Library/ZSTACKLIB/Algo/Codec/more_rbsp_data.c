#include <zstack/bitstream.h>
#include <zstack/types.h>

int more_rbsp_data(struct bitstream* bitstream)
{
	if (bitstream->bitpos >= bitstream->size)
		return FALSE;

	return TRUE;
}
