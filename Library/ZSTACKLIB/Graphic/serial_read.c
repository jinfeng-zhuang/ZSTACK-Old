#include <Windows.h>
#include <zstack/types.h>
#include <zstack/log.h>

u32 serial_read(u32 fd, u8* buffer, u32 length)
{
	DWORD wCount;
	BOOL bReadStat;
	OVERLAPPED overlap;

	memset(&overlap, 0, sizeof(OVERLAPPED));

	bReadStat = ReadFile((HANDLE)fd, buffer, length, &wCount, &overlap);
	if (!bReadStat) {
		if (GetLastError() == ERROR_IO_PENDING) {
			GetOverlappedResult((HANDLE)fd, &overlap, &wCount, TRUE);
		}
		else {
			DEBUG("error read\n");
			return -1;
		}
	}

	return wCount;
}
