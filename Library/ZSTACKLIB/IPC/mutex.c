#include <Windows.h>
#include <process.h>

#include <zstack/thread.h>
#include <zstack/log.h>

mutex_t mutex_create(void)
{
	HANDLE mutex;

	mutex = CreateMutex(NULL, FALSE, NULL);

	return mutex;
}

void mutex_lock(mutex_t mutex)
{
	DWORD ret;

	log(L_DEBUG, "%x enter\n", thread_id());

	ret = WaitForSingleObject(mutex, INFINITE);

	log(L_DEBUG, "%x hold\n", thread_id());
}

void mutex_unlock(mutex_t mutex)
{
	BOOL ret;

	log(L_DEBUG, "%x enter\n", thread_id());

	ret = ReleaseMutex(mutex);

	log(L_DEBUG, "%x release\n", thread_id());
}
