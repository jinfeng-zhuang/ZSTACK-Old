#ifndef VS_NET_H
#define VS_NET_H

#include "types.h"

// continuously transfer
extern int net_connect(const char* host_name, u16 port, u32 msec);
extern int net_transfer(s32 fd, void* tx, void* rx);
extern int net_disconnect(s32 fd);

extern int net_send_once(const char* host, u16 port, void* buffer, u32 length);
extern int net_recv_once(const char* host, u16 port, void* buffer, u32 length);

#endif