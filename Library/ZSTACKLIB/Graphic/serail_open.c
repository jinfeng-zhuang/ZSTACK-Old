#include <Windows.h>
#include <zstack/types.h>

// ref https://blog.csdn.net/u014628654/article/details/45666749

u32 serial_open(char* port)
{
	HANDLE hCom;
	DCB dcb;

	hCom = CreateFileA(port, GENERIC_READ | GENERIC_WRITE,
		0, // don't share
		NULL, // security
		OPEN_EXISTING,
		0, // 0 - sync, 1 - async
		NULL);

	if (hCom == (HANDLE)-1) {
		printf("open com failed\n");
		return NULL;
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

	return (u32)hCom;
}
