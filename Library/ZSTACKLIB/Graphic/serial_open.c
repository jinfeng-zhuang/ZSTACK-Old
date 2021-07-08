#include <Windows.h>
#include <zstack/types.h>
#include <zstack/log.h>

u32 serial_open(char* port)
{
	HANDLE hCom;
	DCB dcb;
	int fd;

	hCom = CreateFileA(port, GENERIC_READ | GENERIC_WRITE,
		0, // don't share
		NULL, // security
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // 0 - sync, 1 - async
		NULL);

	if (hCom == (HANDLE)-1) {
		DEBUG("open %s failed\n", port);
		return -1;
	}

	// 2. configure
	SetupComm(hCom, 1024, 1024);

#if 0
	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadIntervalTimeout = 1000;
	TimeOuts.ReadTotalTimeoutMultiplier = 500;
	TimeOuts.ReadTotalTimeoutConstant = 5000;
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hCom, &TimeOuts);
#endif

	GetCommState(hCom, &dcb);
	dcb.BaudRate = 115200;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = TWOSTOPBITS;
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);

	fd = ((u64)hCom) & 0xFFFFFFFF; // remove warning C4311

	return fd;
}
