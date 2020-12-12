#include <pthread.h>
#include "main.h"

#define CLINET_THREAD_MAX   (10000)

int client_thread_number = 0;
pthread_mutex_t client_thread_count_mutex;

int client_thread_int(void)
{
    pthread_mutex_init(&client_thread_count_mutex, NULL);
}

int client_thread_start(void *(*start_rtn)(void*), void *arg)
{
    pthread_t pid;
    
    if (client_thread_number < CLINET_THREAD_MAX) {
        pthread_create(&pid, NULL, start_rtn, arg);
        client_thread_number++;
        return 0;
    }
    else {
        return -1;
    }
}

int client_thread_end(void)
{
    pthread_mutex_lock(&client_thread_count_mutex);

    if (client_thread_number > 0)
        client_thread_number--;
    
    pthread_mutex_unlock(&client_thread_count_mutex);

}



