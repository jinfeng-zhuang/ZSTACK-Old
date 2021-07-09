#include <string.h>
#include <stdlib.h>
#include <zstack/bitstream.h>
#include <zstack/types.h>

int bitstream_init(struct bitstream* bitstream, unsigned char *buffer, unsigned int length)
{
	unsigned char *bits;
	unsigned int i;
	u32 total;

	if ((NULL == bitstream) || (NULL == buffer) || (0 == length))
		return -1;

	total = length << 3;

	bits = (unsigned char *)malloc(total);
	if (NULL == bits)
		return -1;

	memset(bits, 0, total);

	// to make debug visible and easy
	for (i = 0; i < total; i++) {
		bits[i] = (buffer[i >> 3] & (1 << (7 - (i & 0x7)))) ? '1' : '0';
	}

	bitstream->bitpos = 0;
	bitstream->bits = bits;
	bitstream->size = total;

	return 0;
}
