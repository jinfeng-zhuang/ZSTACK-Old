#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <zstack/log.h>

enum {
    LOG_MODULE_DEFAULT,
    LOG_MODULE_NET,
    LOG_MODULE_DBG,
    LOG_MODULE_AVMIPS,
    LOG_MODULE_HOST,
    LOG_MODULE_UI,
    LOG_MODULE_USER,
    LOG_MODULE_MAX = 31
};

static char *map[LOG_MODULE_MAX] = {
    "default",
    "net",
    "debug",
    "avmips",
    "host",
    "ui",
    "user"
};

static int log_level = LOG_USER;
static unsigned int log_mask[LOG_MODULE_MAX] = {0};

char LOG_buffer[1 << 20];

const char *log_level_name[] = {
    "ERROR",
    "WARNING",
    "INFO",
    "FILE",
    "FUNC",
};

static int find_index_by_name(char *name)
{
    int i;

    for (i = 0; i < LOG_MODULE_MAX; i++) {
        if (0 == strcmp(map[i], name))
            return i;
    }

    return -1;
}

void log_init(int level)
{
    int i;
    char name[32] = {'\0'};

    log_level = level;

    if (log_level >= LOG_FUNC)
        log_level = LOG_FUNC;
    if (log_level <= LOG_ERROR)
        log_level = LOG_ERROR;

    // parse
    

    fflush(stdout);
}

int _log(int lvl, char *filename, char *function, int linenum, const char *fmt, ...)
{
    if ((lvl > log_level) || (lvl < 0) || (NULL == fmt)) {
        return 0;
    }

    LOG_buffer[0] = '\0';
    if (lvl < LOG_USER) {
        sprintf(LOG_buffer, "[%s] ", log_level_name[lvl]);
    }
    else if (lvl == LOG_USER) {
        ;
    }
    else if (lvl == LOG_FILE) {
        sprintf(LOG_buffer, "  [%s] ", filename);
    }
    else {
        sprintf(LOG_buffer, "    [%s:%d] ", function, linenum);
    }
    
    va_list args;
    va_start(args, fmt);
    
#if _WIN32
    _vsnprintf(&LOG_buffer[strlen(LOG_buffer)], sizeof(LOG_buffer) - strlen(LOG_buffer), fmt, args);
#else
    vsnprintf(&LOG_buffer[strlen(LOG_buffer)], sizeof(LOG_buffer) - strlen(LOG_buffer), fmt, args);
#endif

    va_end(args);

    printf("%s", LOG_buffer);
    fflush(stdout);

    if (lvl == LOG_ERROR)
        exit(0xFF);

    return 0;
}

