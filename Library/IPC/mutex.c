#include <Windows.h>
#include <zstack/types.h>

u32 mutex_create(void) {
	HANDLE mutex;
	mutex = CreateMutex(NULL, FALSE, NULL);
	return (u32)mutex;
}

void mutex_lock(u32 mutex) {
    HANDLE _mutex = (HANDLE)mutex;
	WaitForSingleObject(_mutex, INFINITE);
}

void mutex_unlock(u32 mutex) {
    HANDLE _mutex = (HANDLE)mutex;
    ReleaseMutex(_mutex);
}
