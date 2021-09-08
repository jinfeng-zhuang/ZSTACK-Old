#include <zstack/bitstream.h>
#include <zstack/codec.h>
#include "codec.h"

static char debug_str[33];
static int error;

int se_read_se(struct bitstream* bs)
{
	unsigned int count = 0;
	unsigned int value;
	unsigned int symbol;

	while (0 == se_read_u(bs, 1))
		count++;

	bs->bitpos--;

	value = se_read_u(bs, count);
	symbol = se_read_u(bs, 1) ? -1 : 1;

	return value * symbol;
}

unsigned int se_read_u(struct bitstream* bitstream, unsigned int num)
{
	unsigned int i;
	unsigned int value;

	value = 0;

	/* check if overflow */
	if (bitstream->bitpos >= strlen((const char*)bitstream->bits)) {
		error = -1;
		return 0; /* return 0 even if failed */
	}
	else {
		error = 0;
	}

	memcpy(debug_str, &bitstream->bits[bitstream->bitpos], 32);
	debug_str[32] = 0;
	//printf("%s\n", debug_str);

	for (i = 0; i < num; i++) {
		value = value << 1;
		value |= bitstream->bits[bitstream->bitpos] == '1' ? 1 : 0;
		bitstream->bitpos++;
	}

	return value;
}

unsigned int se_read_ue(struct bitstream* bitstream)
{
	unsigned int count = 0;

	while (0 == se_read_u(bitstream, 1))
		count++;

	bitstream->bitpos--;

	return se_read_u(bitstream, count + 1) - 1;
}
