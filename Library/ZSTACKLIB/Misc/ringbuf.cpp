#include <stdio.h>
#include <string.h>
#include <zstack/ringbuf.h>

typedef struct _ringbuf
{
	unsigned char *buffer;
	unsigned int length;

	unsigned int datalen;

	unsigned int head;
	unsigned int tail;
} ringbuf_t;

int g_ringbuf_datalen = 0;

unsigned int ringbuf_get_datalen(ringbuf_t *ringbuf)
{
	return ringbuf->datalen;
}

static unsigned int _ringbuf_get_empty_length(ringbuf_t *ringbuf)
{
	return (ringbuf->length - ringbuf->datalen);
}

static unsigned int _ringbuf_is_enough(ringbuf_t *ringbuf, unsigned int data_to_put)
{
	if (data_to_put <= _ringbuf_get_empty_length(ringbuf))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void ringbuf_init(ringbuf_t *ringbuf, unsigned char *buffer, unsigned int length)
{
	ringbuf->buffer = buffer;
	ringbuf->length = length;

	memset(ringbuf->buffer, 0, ringbuf->length);

	ringbuf->datalen = 0;
	ringbuf->head = 0;
	ringbuf->tail = 0;
}

/*
*      TAIL     HEAD---->
* ---->
*
*  HEAD----->TAIL
*/
unsigned int ringbuf_put(ringbuf_t *ringbuf, unsigned char *buffer, unsigned int length)
{
	unsigned int i;
	volatile unsigned int firstlen;

	if (!_ringbuf_is_enough(ringbuf, length))
	{
		//printf("RINGBUF WARNING: buffer not enough!!! ADJUST PUT\n");
		goto failed;
	}

	// below can combine
	if (ringbuf->tail < ringbuf->head)
	{
		if (ringbuf->length - ringbuf->head >= length)
		{
			for (i = 0; i < length; i++)
			{
				ringbuf->buffer[ringbuf->head + i] = buffer[i];
			}

			ringbuf->head += length;

			if (ringbuf->head == ringbuf->length)
			{
				ringbuf->head = 0;
			}
		}
		else
		{
			firstlen = ringbuf->length - ringbuf->head;

			for (i = 0; i < firstlen; i++)
			{
				ringbuf->buffer[ringbuf->head + i] = buffer[i];
			}

			for (i = 0; i < length - firstlen; i++)
			{
				ringbuf->buffer[i] = buffer[i + firstlen];
			}

			ringbuf->head = length - firstlen;
		}
	}
	else if (ringbuf->tail == ringbuf->head)
	{
		if (ringbuf->length - ringbuf->head >= length)
		{
			for (i = 0; i < length; i++)
			{
				ringbuf->buffer[ringbuf->head + i] = buffer[i];
			}

			ringbuf->head += length;

			if (ringbuf->head == ringbuf->length)
			{
				ringbuf->head = 0;
			}
		}
		else
		{
			firstlen = ringbuf->length - ringbuf->head;

			for (i = 0; i < firstlen; i++)
			{
				ringbuf->buffer[ringbuf->head + i] = buffer[i];
			}

			for (i = 0; i < length - firstlen; i++)
			{
				ringbuf->buffer[i] = buffer[i + firstlen];
			}

			ringbuf->head = length - firstlen;
		}
	}
	else
	{
		for (i = 0; i < length; i++)
		{
			ringbuf->buffer[i + ringbuf->head] = buffer[i];
		}

		ringbuf->head += length;
	}

	ringbuf->datalen += length;

	return length;

failed:
	return 0;
}

/*
*      TAIL----->HEAD
*
*     HEAD     TAIL----->
* --->
*/
unsigned int ringbuf_get(ringbuf_t *ringbuf, unsigned char *buffer, unsigned int length)
{
	unsigned int i;
	unsigned int firstlen;
	unsigned int realget;

	// get real length
	if (ringbuf_get_datalen(ringbuf) > length)
	{
		realget = length;
	}
	else
	{
		realget = ringbuf_get_datalen(ringbuf);
	}

	if (ringbuf->tail < ringbuf->head)
	{
		for (i = 0; i < realget; i++)
		{
			buffer[i] = ringbuf->buffer[ringbuf->tail + i];
		}

		ringbuf->tail += realget;
	}

	// below can combine
	else if (ringbuf->tail == ringbuf->head)
	{
		if (ringbuf->length - ringbuf->tail >= realget)
		{
			for (i = 0; i < realget; i++)
			{
				buffer[i] = ringbuf->buffer[ringbuf->tail + i];
			}

			ringbuf->tail += realget;

			if (ringbuf->tail == ringbuf->length)
			{
				ringbuf->tail = 0;
			}
		}
		else
		{
			firstlen = ringbuf->length - ringbuf->tail;

			for (i = 0; i < firstlen; i++)
			{
				buffer[i] = ringbuf->buffer[ringbuf->tail + i];
			}

			for (i = 0; i < realget - firstlen; i++)
			{
				buffer[i + firstlen] = ringbuf->buffer[i];
			}

			ringbuf->tail = i;
		}
	}
	else
	{
		if (ringbuf->length - ringbuf->tail >= realget)
		{
			for (i = 0; i < realget; i++)
			{
				buffer[i] = ringbuf->buffer[ringbuf->tail + i];
			}

			ringbuf->tail += realget;

			if (ringbuf->tail == ringbuf->length)
			{
				ringbuf->tail = 0;
			}
		}
		else
		{
			firstlen = ringbuf->length - ringbuf->tail;

			for (i = 0; i < firstlen; i++)
			{
				buffer[i] = ringbuf->buffer[ringbuf->tail + i];
			}

			for (i = 0; i < realget - firstlen; i++)
			{
				buffer[i + firstlen] = ringbuf->buffer[i];
			}

			ringbuf->tail = i;
		}
	}

	ringbuf->datalen -= realget;

	return realget;
}

// wp < rp:
// wp -> keep 1 slot -> rp
unsigned int ringbuf_measure(struct ring *r, unsigned int request)
{
    unsigned int remain;
    unsigned int response;

    if (r->wp >= r->rp) {
        remain = r->wp - r->rp;
        response = (request > remain) ? remain : request;
        return response;
    }
    else{
        remain = r->end - r->rp;
        response = (request > remain) ? remain : request;
        return response;
    }
}

unsigned int ringbuf_get_datalen(struct ring *r)
{
    unsigned int remain;
    unsigned int remain2;

    if (r->wp >= r->rp) {
        remain = r->wp - r->rp;
        return remain;
    }
    else{
        remain = r->end - r->rp;
        remain2 = r->wp - r->start;
        return remain + remain2;
    }
}
