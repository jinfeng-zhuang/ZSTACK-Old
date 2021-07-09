#ifndef ZSTACK_BITSTREAM_H
#define ZSTACK_BITSTREAM_H

struct bitstream {
	unsigned int bitpos;
	unsigned char *bits;
	unsigned int size;
};

extern struct bitstream* bitstream_malloc(unsigned char* buffer, unsigned int length);
extern int bitstream_free(struct bitstream* bs);

#endif