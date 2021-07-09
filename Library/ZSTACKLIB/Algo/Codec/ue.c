#include <zstack/bitstream.h>
#include "codec.h"

unsigned int ue(struct bitstream* bitstream)
{
	unsigned int count = 0;

	while (0 == u(bitstream, 1))
		count++;

	bitstream->bitpos--;

	return u(bitstream, count + 1) - 1;
}
