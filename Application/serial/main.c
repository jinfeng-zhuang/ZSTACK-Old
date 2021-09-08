#include <zstack/zstack.h>
#include "main.h"
#include <stdio.h>
#include <Windows.h>

enum {
	STATUS_FREE = 0,
	STATUS_BUSY
};

struct application app;

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

		c = getch();
		ret = serial_write(serial_fd, &c, 1);
		if (ret == -1) {
			//serial_fd = -1;
		}
	}
}

int main(int argc, char* argv[])
{
    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }
	
	log_init(app.param.log_config);
	
	if (app.param.list_flag) {
		serial_port_list();
	}
	else {
		thread_create(thread_rx, 0, 0);
		thread_create(thread_tx, 0, 0);

		serial_fd = serial_open(app.param.port);
		if (-1 == serial_fd) {
			DEBUG("Port %s has been taken\n", argv[1]);
		}

		while (1) {
			msleep(100);
		}
	}
	
	return 0;
}
