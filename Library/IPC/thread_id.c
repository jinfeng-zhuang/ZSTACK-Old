#include <zstack/thread.h>

#include <Windows.h>
#include <process.h>

unsigned int thread_id(void)
{
	return GetCurrentThreadId();
}