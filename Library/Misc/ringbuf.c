#include <stdio.h>
#include <string.h>
#include <zstack/ringbuf.h>

unsigned int ringbuf_empty(struct ringbuffer* r)
{
	return (r->length - ringbuf_datalen(r));
}

unsigned int ringbuf_datalen(struct ringbuffer *r)
{
	if (r->wp >= r->rp) {
		return (r->wp - r->rp);
	}
	else {
		return (r->length - r->rp) + r->wp;
	}
}

float ringbuf_datalen_percent(struct ringbuffer* r)
{
	return ((float)ringbuf_datalen(r) / r->length) * 100;
}

static unsigned int _ringbuf_get_empty_length(struct ringbuffer *ringbuf)
{
	return (ringbuf->length - ringbuf_datalen(ringbuf));
}

static unsigned int _ringbuf_is_enough(struct ringbuffer *ringbuf, unsigned int data_to_put)
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

int ringbuf_init(struct ringbuffer *ringbuf, unsigned char *buffer, unsigned int length)
{
	if ((ringbuf == NULL) || (NULL == buffer) || (0 == length))
		return -1;

	ringbuf->buffer = buffer;
	ringbuf->length = length;

	memset(ringbuf->buffer, 0, ringbuf->length);

	ringbuf->wp = 0;
	ringbuf->rp = 0;

	return 0;
}

/*
*      rp     wp---->
* ---->
*
*  wp----->rp
*/
unsigned int ringbuf_put(struct ringbuffer *ringbuf, unsigned char *buffer, unsigned int length)
{
	unsigned int i;
	volatile unsigned int firstlen;

	if (!_ringbuf_is_enough(ringbuf, length))
	{
		//printf("RINGBUF WARNING: buffer not enough!!! ADJUST PUT\n");
		goto failed;
	}

	// below can combine
	if (ringbuf->rp < ringbuf->wp)
	{
		if (ringbuf->length - ringbuf->wp >= length)
		{
			for (i = 0; i < length; i++)
			{
				ringbuf->buffer[ringbuf->wp + i] = buffer[i];
			}

			ringbuf->wp += length;

			if (ringbuf->wp == ringbuf->length)
			{
				ringbuf->wp = 0;
			}
		}
		else
		{
			firstlen = ringbuf->length - ringbuf->wp;

			for (i = 0; i < firstlen; i++)
			{
				ringbuf->buffer[ringbuf->wp + i] = buffer[i];
			}

			for (i = 0; i < length - firstlen; i++)
			{
				ringbuf->buffer[i] = buffer[i + firstlen];
			}

			ringbuf->wp = length - firstlen;
		}
	}
	else if (ringbuf->rp == ringbuf->wp)
	{
		if (ringbuf->length - ringbuf->wp >= length)
		{
			for (i = 0; i < length; i++)
			{
				ringbuf->buffer[ringbuf->wp + i] = buffer[i];
			}

			ringbuf->wp += length;

			if (ringbuf->wp == ringbuf->length)
			{
				ringbuf->wp = 0;
			}
		}
		else
		{
			firstlen = ringbuf->length - ringbuf->wp;

			for (i = 0; i < firstlen; i++)
			{
				ringbuf->buffer[ringbuf->wp + i] = buffer[i];
			}

			for (i = 0; i < length - firstlen; i++)
			{
				ringbuf->buffer[i] = buffer[i + firstlen];
			}

			ringbuf->wp = length - firstlen;
		}
	}
	else
	{
		for (i = 0; i < length; i++)
		{
			ringbuf->buffer[i + ringbuf->wp] = buffer[i];
		}

		ringbuf->wp += length;
	}

	return length;

failed:
	return 0;
}

/*
*      rp----->wp
*
*     wp     rp----->
* --->
*/
unsigned int ringbuf_get(struct ringbuffer *ringbuf, unsigned char *buffer, unsigned int length)
{
	unsigned int i;
	unsigned int firstlen;
	unsigned int realget;

	// get real length
	if (ringbuf_datalen(ringbuf) > length)
	{
		realget = length;
	}
	else
	{
		realget = ringbuf_datalen(ringbuf);
	}

	if (ringbuf->rp < ringbuf->wp)
	{
		for (i = 0; i < realget; i++)
		{
			buffer[i] = ringbuf->buffer[ringbuf->rp + i];
		}

		ringbuf->rp += realget;
	}

	// below can combine
	else if (ringbuf->rp == ringbuf->wp)
	{
		if (ringbuf->length - ringbuf->rp >= realget)
		{
			for (i = 0; i < realget; i++)
			{
				buffer[i] = ringbuf->buffer[ringbuf->rp + i];
			}

			ringbuf->rp += realget;

			if (ringbuf->rp == ringbuf->length)
			{
				ringbuf->rp = 0;
			}
		}
		else
		{
			firstlen = ringbuf->length - ringbuf->rp;

			for (i = 0; i < firstlen; i++)
			{
				buffer[i] = ringbuf->buffer[ringbuf->rp + i];
			}

			for (i = 0; i < realget - firstlen; i++)
			{
				buffer[i + firstlen] = ringbuf->buffer[i];
			}

			ringbuf->rp = i;
		}
	}
	else
	{
		if (ringbuf->length - ringbuf->rp >= realget)
		{
			for (i = 0; i < realget; i++)
			{
				buffer[i] = ringbuf->buffer[ringbuf->rp + i];
			}

			ringbuf->rp += realget;

			if (ringbuf->rp == ringbuf->length)
			{
				ringbuf->rp = 0;
			}
		}
		else
		{
			firstlen = ringbuf->length - ringbuf->rp;

			for (i = 0; i < firstlen; i++)
			{
				buffer[i] = ringbuf->buffer[ringbuf->rp + i];
			}

			for (i = 0; i < realget - firstlen; i++)
			{
				buffer[i + firstlen] = ringbuf->buffer[i];
			}

			ringbuf->rp = i;
		}
	}

	return realget;
}
