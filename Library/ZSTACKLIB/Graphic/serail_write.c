#include <Windows.h>
#include <zstack/types.h>
#include <zstack/log.h>

u32 serial_write(u32 fd, u8* buffer, u32 length)
{
	u32 dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError((HANDLE)fd, &dwErrorFlags, &ComStat);
	bWriteStat = WriteFile((HANDLE)fd, buffer, length, &dwBytesWrite, NULL);
	if (!bWriteStat) {
		DEBUG("write failed\n");
		return -1;
	}

	return dwBytesWrite;
}
