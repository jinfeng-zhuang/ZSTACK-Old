#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <zstack/types.h>
#include <zstack/log.h>

#define PORTNAME_SIZE	(16)

u32 serial_open(u8 id)
{
	HANDLE hCom;
	DCB dcb;
	int fd;
	COMMTIMEOUTS TimeOuts;
	char port[PORTNAME_SIZE];

	memset(port, 0, PORTNAME_SIZE);
	if (id <= 10) {
		snprintf(port, PORTNAME_SIZE, "COM%d", id);
	}
	else {
		snprintf(port, PORTNAME_SIZE, "\\\\.\\COM%d", id);
	}

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

	TimeOuts.ReadIntervalTimeout = 1000;
	TimeOuts.ReadTotalTimeoutMultiplier = 500;
	TimeOuts.ReadTotalTimeoutConstant = 5000;
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hCom, &TimeOuts);

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
