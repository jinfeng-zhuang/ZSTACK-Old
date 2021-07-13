#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>
#include <zstack/ipc.h>
#include <zstack/time.h>

#define LOG_SIZE (128)

static FILE* fp = NULL;
static unsigned int mutex;

int log2file(const char* fmt, ...)
{
    struct time tm;
    va_list     args;
    char        log_buffer[LOG_SIZE];

	if (NULL == fp) {
		fp = fopen("log.txt", "w+");

		mutex = mutex_create();
	}

	if (fp) {
        get_time(&tm);

        if (mutex)
            mutex_lock(mutex);

        memset(log_buffer, 0, LOG_SIZE);
        va_start(args, fmt);
        vsnprintf(log_buffer, LOG_SIZE, fmt, args);
        va_end(args);

        if (mutex)
            mutex_unlock(mutex);

        fprintf(fp, "[%d-%d_%d:%d] %s", tm.month, tm.day, tm.hour, tm.min, log_buffer);
        fflush(fp);
	}

	return strlen(log_buffer);
}
