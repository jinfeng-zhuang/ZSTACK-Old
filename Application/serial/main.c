#include <zstack/zstack.h>
#include <stdio.h>
#include <conio.h>

enum {
	STATUS_FREE = 0,
	STATUS_BUSY
};

static int serial_fd = -1;
static int status = 0;

int thread_rx(void* arg)
{
	u32 ret;
	u8 buffer[1024];

	while (1) {
		if (-1 == serial_fd) {
			msleep(100);
			continue;
		}

		memset(buffer, 0, 1024);
		ret = serial_read(serial_fd, buffer, 1024 - 1);
		if (ret != -1) {
			DEBUG(buffer);
		}
		else {
			//serial_fd = -1;
		}
	}
}

int thread_tx(void* arg)
{
	u32 ret;
	u8 buffer[1024];
	char c;

	while (1) {
		if (-1 == serial_fd) {
			msleep(100);
			continue;
		}

		memset(buffer, 0, 1024);
		fgets(buffer, 1024 - 1, stdin);
		ret = serial_write(serial_fd, buffer, strlen(buffer));
		if (ret == -1) {
			//serial_fd = -1;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc == 1) {
		serial_port_list();
	}
	else {
		thread_create(thread_rx, 0, 0);
		thread_create(thread_tx, 0, 0);

		serial_fd = serial_open(argv[1]);
		if (-1 == serial_fd) {
			DEBUG("Port %s has been taken\n", argv[1]);
		}

		while (1) {
			msleep(100);
		}
	}
}
