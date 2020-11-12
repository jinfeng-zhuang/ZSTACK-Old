#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <zstack/log.h>

#define LOG_OUTPUT printf

static char *map[LOG_MODULE_MAX] = {
    "default",
    "app",
    "ui",
    "misc",
    "net",
    "avmips",
    "algo"
};

static unsigned int log_mask[LOG_MODULE_MAX] = {0};
static int log_inited = 0;
static char LOG_buffer[1 << 20];

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

    ret = sscanf(config, "%[^:]:%d", module_name, &level);
    if (2 != ret) {
        LOG_OUTPUT("log config '%s' is not '<key>:<value>' format\n", config);
        return -1;
    }

    index = find_index_by_name(module_name);
    if (-1 == index) {
        LOG_OUTPUT("log config name '%s' not valid\n", config);
        return -1;
    }

    log_mask[index] = correct_level(level);

    return 0;
}

// "default:x, ui:x, net:x"
void log_init(char *config)
{
    int i;
    int j;
    int module_start;
    int module_end;
    char module_name[32] = {'\0'};
    int config_len;

    if (config == NULL)
        config = "default:2";

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
    LOG_OUTPUT("log config not correct, no log in the system\n");
    log_inited = 0;
}

int _log(int module, int lvl, char *filename, char *function, int linenum, const char *fmt, ...)
{
    if (! log_inited)
        return -1;

    lvl = correct_level(lvl);

    if (lvl > log_mask[module]) {
        return 0;
    }

    LOG_buffer[0] = '\0';

    if (lvl == LOG_ERROR) {
        sprintf(LOG_buffer, "[ERROR] [%s:%d] ", function, linenum);
    }
    else if (lvl == LOG_WARNING) {
        sprintf(LOG_buffer, "[WARNING] [%s:%d] ", function, linenum);
    }
    else if (lvl == LOG_USER) {
        ;
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

    LOG_OUTPUT("%s", LOG_buffer);
    fflush(stdout);

    if (lvl == LOG_ERROR)
        exit(0xFF);

    return 0;
}

void log_init_test(void)
{
    log_init("default:3,ui:2");
}
