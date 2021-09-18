#include <zstack/zstack.h>
#include <stdlib.h>

struct queue {
	struct ringbuffer ring;
	int item_size;
	unsigned int mutex;
};

// user define this structure
struct qMsg; // derived from base which contains mutex, size

// user malloc the buffer as array???
// lib just do lock & unlock ???
struct queue *queue_create(void *buffer, int size, int count)
{
	unsigned int addr;

	addr = (unsigned int)malloc(size * count);

	return addr;
}

int queue_delete(struct queue* q)
{
	return 0;
}

int queue_add(void* arg)
{
	return 0;
}

int queue_get(void* arg)
{
	return 0;
}
