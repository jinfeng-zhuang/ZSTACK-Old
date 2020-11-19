#ifndef ZSTACK_PLATFORM_H
#define ZSTACK_PLATFORM_H

#define MSG_SET_HANDLE  (0)
#define MSG_SET_DATA    (1)

extern int Platform_Init(const char *classname, int (*callback)(int evt, void *in, void *out));

#endif
