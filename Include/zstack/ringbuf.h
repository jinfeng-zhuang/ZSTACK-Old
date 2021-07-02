//
// if buffer is NULL, just update wp, rp
//

#ifndef ZSTACK_RINGBUF_H
#define ZSTACK_RINGBUF_H

#include "types.h"

struct ringbuffer {
	unsigned char *buffer;
	unsigned int length;
    
    unsigned int start;
	unsigned int end;
	unsigned int wp;
	unsigned int rp;

	u32 datalen;
	u32 remain;
};

extern int ringbuf_init(struct ringbuffer *r, unsigned char *buffer, unsigned int length);

extern unsigned int ringbuf_put(struct ringbuffer *r, unsigned char *buffer, unsigned int length);
extern unsigned int ringbuf_get(struct ringbuffer *r, unsigned char *buffer, unsigned int length);

extern unsigned int ringbuf_datalen(struct ringbuffer *r);
extern unsigned int ringbuf_empty(struct ringbuffer *r);

#endif
