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

	memset(&overlap, 0, sizeof(OVERLAPPED));

	PurgeComm((HANDLE)fd, PURGE_RXABORT);

	ClearCommError((HANDLE)fd, &dwErrorFlags, &ComStat);

	bWriteStat = WriteFile((HANDLE)fd, buffer, length, &dwBytesWrite, &overlap);
	if (!bWriteStat) {
		if (GetLastError() == ERROR_IO_PENDING) {
			GetOverlappedResult((HANDLE)fd, &overlap, &dwBytesWrite, TRUE);
		}
		else {
			DEBUG("error write: %d\n", GetLastError());
			return -1;
		}
	}

	return dwBytesWrite;
}
