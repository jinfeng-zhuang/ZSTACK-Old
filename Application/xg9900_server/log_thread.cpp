#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>

const char *log_file = "/tmp/x9900.log";

pthread_mutex_t log_mutex;
pthread_cond_t log_cond;

#define LOG_BUFFER_SIZE (1<<20)

#define LOG2FILE_THROSHOLD (1<<10)

char log_buffer[1<<10] = {'\0'};

int log2buffer(const char *fmt, ...)
{
    va_list args;
    int log_buffer_count;
    int log_buffer_remain;

    pthread_mutex_lock(&log_mutex);

    log_buffer_count = strlen(log_buffer);

    if (log_buffer_count < LOG_BUFFER_SIZE) {
        log_buffer_remain = LOG_BUFFER_SIZE - log_buffer_count;
        
        va_start(args, fmt);
        vsnprintf(&log_buffer[log_buffer_count], log_buffer_remain, fmt, args);
        va_end(args);

        log_buffer_count = strlen(log_buffer);

        if (log_buffer_count >= LOG2FILE_THROSHOLD) {
            pthread_cond_signal(&log_cond);
        }
    }

    pthread_mutex_unlock(&log_mutex);
}

void *log_dump_thread(void *arg)
{
    FILE *fp = NULL;
    struct timespec timeout;
    int ret;
    int byte2write;

    fp = fopen(log_file, "w+");
    if (NULL == fp) {
        printf("log_dump_thread: failed to open log file\n");
        return NULL;
    }
    
    while (1) {
        pthread_mutex_lock(&log_mutex);

        gettimeofday(&timeout, NULL);
        timeout.tv_sec += 1;
        pthread_cond_timedwait(&log_cond, &log_mutex, &timeout);

        byte2write = (strlen(log_buffer) > LOG2FILE_THROSHOLD) ? LOG2FILE_THROSHOLD : strlen(log_buffer);

        //printf("byte2write %d\n", byte2write);

        if (byte2write > 0) {
            printf("%s", log_buffer);

            // TODO: can be put in mutex area ?
            //ret = fwrite(log_buffer, 1, strlen(log_buffer), fp);
        
            log_buffer[0] = '\0';
        }

        pthread_mutex_unlock(&log_mutex);
    }
}

int log_thread_init(void)
{
    pthread_t pid;
    
    pthread_mutex_init(&log_mutex, NULL);
    pthread_cond_init(&log_cond, NULL);

    pthread_create(&pid, NULL, log_dump_thread, (void *)NULL);
}

