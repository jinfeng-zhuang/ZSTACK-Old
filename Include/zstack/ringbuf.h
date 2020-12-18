#ifndef ZSTACK_RINGBUF_H
#define ZSTACK_RINGBUF_H

struct ring {
    unsigned int start;
	unsigned int end;
	unsigned int wp;
	unsigned int rp;
};

struct ringbuffer_t {
	unsigned char *buffer;
    
    unsigned int start;
	unsigned int end;
	unsigned int wp;
	unsigned int rp;
};

void ringbuf_init(struct ringbuffer_t *ringbuf, unsigned char *buffer, unsigned int length);
unsigned int ringbuf_put(struct ringbuffer_t *ringbuf, unsigned char *buffer, unsigned int length);
unsigned int ringbuf_get(struct ringbuffer_t *ringbuf, unsigned char *buffer, unsigned int length);
unsigned int ringbuf_get_datalen(struct ringbuffer_t *ringbuf);

extern unsigned int ringbuf_get_datalen(struct ring *r);
extern unsigned int ringbuf_measure(struct ring *r, unsigned int request);

#endif
