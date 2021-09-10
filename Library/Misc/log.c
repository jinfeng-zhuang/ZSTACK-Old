#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <zstack/time.h>
#include <zstack/log.h>
#include <zstack/misc.h>

#define LOG_MODULE_NAME_MAX 80
#define LOG_MOODULE_MAX 100

static int log_inited = 0;

static char tmp_buffer[10 << 20];

static FILE* logfile = NULL;
static int flag_time = 0;
static int flag_prefix = 0;

static struct log_module {
    char name[LOG_MODULE_NAME_MAX + 1];
    int init;
} log_modules[LOG_MOODULE_MAX];

// if match, return 0, otherwise return the empty slot index
static int module_match(const char* name, int *idx)
{
    int i;

    for (i = 0; i < ARRAY_SIZE(log_modules); i++) {
        if (0 == strcmp(name, log_modules[i].name)) {
            return 1;
        }

        if (0 == log_modules[i].init)
            break;
    }

    *idx = i;

    return 0;
}

static int add_to_log_mask(char *name)
{
    int ret;
    int index;
    char filename[FILENAME_MAX];

    if (NULL == name)
        return -1;

    // find a empty slot
    ret = module_match(name, &index);
    if (0 == ret) {
        strcpy(log_modules[index].name, name);
        log_modules[index].init = 1;
    }

    // check if it is 'file'
    if (0 == strcmp(name, "file")) {
        struct time time;

        get_time(&time);
        memset(filename, 0, sizeof(filename));

        snprintf(filename, sizeof(filename), "%d-%02d-%02d_%02d-%02d-%02d.log",
            time.year,
            time.month,
            time.day,
            time.hour,
            time.min,
            time.sec
            );

        logfile = fopen(filename, "w");

        printf("LOG FILE: %s\n", filename);
    }

    // check if it is 'time'
    if (0 == strcmp(name, "time")) {
        flag_time = 1;
    }

    if (0 == strcmp(name, "prefix")) {
        flag_prefix = 1;
    }

    return 0;
}

// "default,ui,net"
void log_init(const char *config)
{
    int i;
    int j;
    char module_name[LOG_MODULE_NAME_MAX + 1] = {'\0'};
    size_t config_len;

    if ((config == NULL) || ('\0' == config[0])) {
        printf("log: enable default main\n");
        config = (char*)"main";
    }

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
            if (j >= LOG_MODULE_NAME_MAX) {
                goto FAILED;
            }
        }
    }

    if (0 != add_to_log_mask(module_name))
        goto FAILED;

    log_inited = 1;

    return;

FAILED:
    log_inited = 0;
}

int _log(const char * name, int force, const char *fmt, ...)
{
    int index;

    // Check if module inited
    if (! log_inited) {
        log_init(NULL);
    }

    // Check the message
    if (! force) {
        if (! module_match(name, &index)) {
            return 0;
        }
    }

    tmp_buffer[0] = '\0';

    // Add prefix
    // check time flag
    if (flag_time) {
        struct time t;
        get_time(&t);
        snprintf(tmp_buffer, sizeof(tmp_buffer), "[%02d:%02d:%02d] ", t.hour, t.min, t.sec);
    }

    if (flag_prefix)
        snprintf(&tmp_buffer[strlen(tmp_buffer)], sizeof(tmp_buffer) - strlen(tmp_buffer), "[%s] ", name);

    // format the string buffer
    va_list args;
    va_start(args, fmt);
    
    vsnprintf(&tmp_buffer[strlen(tmp_buffer)], sizeof(tmp_buffer) - strlen(tmp_buffer), fmt, args);

    va_end(args);

    // Output to terminal
    printf("%s", tmp_buffer);
    fflush(stdout);

    // Output to file
    if (logfile) {
        fprintf(logfile, "%s", tmp_buffer);
        fflush(logfile);
    }

    return 0;
}
