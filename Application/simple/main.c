#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>
#include <stdio.h>

extern void serial_port_list(void);

int main(int argc, char* argv[])
{
	log_init("net_connect,dbg_init,main,time,file");

	DEBUG("Hello World\n");

	dbg_init("0.0.0.0");

	serial_port_list();
}
