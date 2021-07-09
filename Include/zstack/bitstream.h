#ifndef ZSTACK_BITSTREAM_H
#define ZSTACK_BITSTREAM_H

struct bitstream {
	unsigned int bitpos;
	unsigned char *bits;
	unsigned int size;
};

extern int bitstream_init(struct bitstream *bitstream, unsigned char *buffer, unsigned int length);

#endif