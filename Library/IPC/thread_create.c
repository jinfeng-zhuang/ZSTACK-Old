#include <zstack/ipc.h>

#include <Windows.h>
#include <process.h>

int thread_create(int (*entry)(void*), void *args)
{
	HANDLE hThread;

	hThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)entry, NULL, 0, NULL);
	if (NULL == hThread) {
		return -1;
	}

	return 0;
}
