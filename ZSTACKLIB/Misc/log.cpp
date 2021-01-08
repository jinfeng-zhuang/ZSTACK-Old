#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <zstack/log.h>

#ifdef _WIN32
#include <Windows.h>

#define snprintf _snprintf
#else
#endif

#define LOG_OUTPUT printf
#define DEFAULT_LOG_CONFIG "default:2"

static char *map[LOG_MODULE_MAX] = {
    "default",
    "app",
    "ui",
    "misc",
    "net",
    "avmips",
    "algo",
    "file",
    "time",
};

static char *prefix[] = {
    "\033[31m[ERROR]",
    "\033[33m[WARNING]",
    "",
    "\033[32m"
};

static unsigned int log_mask[LOG_MODULE_MAX] = {0};
static int log_inited = 0;
static FILE *logfile = NULL;
static char LOG_buffer[1 << 20];
static int flag_time = 0;

static int find_index_by_name(char *name)
{
    int i;

    for (i = 0; i < LOG_MODULE_MAX; i++) {
        if (0 == strcmp(map[i], name))
            return i;
    }

    return -1;
}

static int correct_level(int level)
{
    if (level > LOG_DEBUG)
        level = LOG_DEBUG;
    else if (level < LOG_ERROR)
        level = LOG_ERROR;
    else
        ;

    return level;
}

static int add_to_log_mask(char *config)
{
    char module_name[32];
    int level;
    int index;
    int ret;
    char filename[FILENAME_MAX];

    ret = sscanf(config, "%[^:]:%d", module_name, &level);
    if (2 != ret) {
        //LOG_OUTPUT("log config '%s' is not '<key>:<value>' format\n", config);
        return -1;
    }

    index = find_index_by_name(module_name);
    if (-1 == index) {
        LOG_OUTPUT("log config name '%s' not valid\n", config);
        return -1;
    }

    if ((index == LOG_MODULE_FILE) && (level != 0)) {
#ifdef _WIN32
        SYSTEMTIME time;
        GetLocalTime(&time);
        snprintf(filename, sizeof(filename), "%d-%02d-%02d_%02d-%02d-%02d.log",
            time.wYear,
            time.wMonth,
            time.wDay,
            time.wHour,
            time.wMinute,
            time.wSecond
            );
#else
        snprintf(filename, sizeof(filename), "default.log");
#endif
        logfile = fopen(filename, "w");
    }

    if ((index == LOG_MODULE_TIME) && (level != 0)) {
        flag_time = 1;
    }

    log_mask[index] = correct_level(level);

    return 0;
}

// "default:x, ui:x, net:x"
void log_init(char *config)
{
    int i;
    int j;
    char module_name[32] = {'\0'};
    int config_len;

    if (config == NULL)
        config = DEFAULT_LOG_CONFIG;

    // parse
    config_len = strlen(config);

    for (i = 0, j = 0; i < config_len; i++) {
        if (config[i] == ',') {

            j = 0;

            if (0 != add_to_log_mask(module_name))
                goto FAILED;

            memset(module_name, 0, sizeof(module_name));
        }
        else if (' ' == config[i]){
            continue;
        }
        else {
            module_name[j++] = config[i];
            if (j >= 31) {
                goto FAILED;
            }
        }
    }

    if (0 != add_to_log_mask(module_name))
        goto FAILED;

    log_inited = 1;

    return;

FAILED:
    //LOG_OUTPUT("log config: \"%s\"\n", DEFAULT_LOG_CONFIG);
    log_inited = 0;
}

int _log(int module, int lvl, char *filename, char *function, int linenum, const char *fmt, ...)
{
    char _prefix[128] = {0};

    if (! log_inited) {
        log_init(NULL);
    }

    lvl = correct_level(lvl);

    if ((lvl >= LOG_USER) && (lvl > log_mask[module])) {
        return 0;
    }

    if (flag_time) {
#ifdef _WIN32
        SYSTEMTIME time;
        GetSystemTime(&time);
        snprintf(_prefix, sizeof(_prefix), "%02d:%02d:%02d)", time.wHour, time.wMinute, time.wSecond);
#endif
    }

    if (! logfile)
        strcpy(&_prefix[strlen(_prefix)], prefix[lvl]);

    LOG_buffer[0] = '\0';

    if (lvl == LOG_ERROR) {
        sprintf(LOG_buffer, "%s [%s:%d] ", _prefix, function, linenum);
    }
    else if (lvl == LOG_WARNING) {
        sprintf(LOG_buffer, "%s [%s:%d] ", _prefix, function, linenum);
    }
    else if (lvl == LOG_USER) {
        sprintf(LOG_buffer, "%s ", _prefix);;
    }
    else {
        sprintf(LOG_buffer, "%s    [%s:%d] ", _prefix, function, linenum);
    }
    
    va_list args;
    va_start(args, fmt);
    
#if _WIN32
    _vsnprintf(&LOG_buffer[strlen(LOG_buffer)], sizeof(LOG_buffer) - strlen(LOG_buffer), fmt, args);
#else
    vsnprintf(&LOG_buffer[strlen(LOG_buffer)], sizeof(LOG_buffer) - strlen(LOG_buffer), fmt, args);
#endif

    va_end(args);

    LOG_OUTPUT("%s%s", LOG_buffer, (lvl == LOG_USER) ? "" : "\033[0m");
    fflush(stdout);

    if (logfile) {
        fprintf(logfile, "%s", LOG_buffer);
        fflush(logfile);
    }

    if (lvl == LOG_ERROR)
        exit(0xFF);

    return 0;
}

void log_init_test(void)
{
    log_init("default:3,ui:2");
}
