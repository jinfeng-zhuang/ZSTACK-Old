#ifndef MAIN_H
#define MAIN_H

extern int client_thread_int(void);
extern int client_thread_start(void *(*start_rtn)(void*), void *arg);
extern int client_thread_end(void);

extern int log_thread_init(void);
extern int log2buffer(const char *fmt, ...);
extern void *log_dump_thread(void *arg);

#endif
