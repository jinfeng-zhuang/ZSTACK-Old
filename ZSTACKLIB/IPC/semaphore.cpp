#include <zstack/thread.h>

#include <Windows.h>
#include <process.h>

sem_t sem_create(int count)
{
	return CreateSemaphore(NULL, 0, count, NULL);
}

int sem_wait(sem_t sem, unsigned int ms)
{
	if (-1 == ms) {
		ms = INFINITE;
	}

	return WaitForSingleObject(sem, ms);
}

int sem_post(sem_t sem)
{
	ReleaseSemaphore(sem, 1, NULL);

	return 0;
}
