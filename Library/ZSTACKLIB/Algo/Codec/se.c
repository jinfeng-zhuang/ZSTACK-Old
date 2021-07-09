#include <zstack/bitstream.h>
#include "codec.h"

int se(struct bitstream* bitstream)
{
	unsigned int count = 0;
	unsigned int value;
	unsigned int symbol;

	while (0 == u(bitstream, 1))
		count++;

	bitstream->bitpos--;

	value = u(bitstream, count);
	symbol = u(bitstream, 1) ? -1 : 1;

	return value * symbol;
}
