#include <Windows.h>
#include <zstack/types.h>
#include <zstack/log.h>

u32 serial_write(u32 fd, u8* buffer, u32 length)
{
	u32 dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	OVERLAPPED overlap;
	HANDLE handle = (HANDLE)(u64)fd; // remove warning C4312

	memset(&overlap, 0, sizeof(OVERLAPPED));

	PurgeComm(handle, PURGE_RXABORT);

	ClearCommError(handle, &dwErrorFlags, &ComStat);

	bWriteStat = WriteFile(handle, buffer, length, &dwBytesWrite, &overlap);
	if (!bWriteStat) {
		if (GetLastError() == ERROR_IO_PENDING) {
			GetOverlappedResult(handle, &overlap, &dwBytesWrite, TRUE);
		}
		else {
			DEBUG("error write: %d\n", GetLastError());
			return -1;
		}
	}

	return dwBytesWrite;
}
