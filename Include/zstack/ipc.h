#ifndef ZSTACK_IPC_H
#define ZSTACK_IPC_H

typedef int (*thread_entry)(void*);

// ignore stacksize on windows
extern int thread_create(thread_entry entry, unsigned int stacksize, void* args);

#endif