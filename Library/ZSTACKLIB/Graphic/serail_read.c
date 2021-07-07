#include <Windows.h>
#include <zstack/types.h>
#include <zstack/log.h>

u32 serial_read(u32 fd, u8* buffer, u32 length)
{
	DWORD wCount;
	BOOL bReadStat;

	bReadStat = ReadFile((HANDLE)fd, buffer, length, &wCount, NULL);
	if (!bReadStat) {
		DEBUG("read failed\n");
		return -1;
	}

	return wCount;
}
