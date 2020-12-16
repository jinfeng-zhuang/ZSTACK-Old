#ifndef MAIN_H
#define MAIN_H

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char path[FILENAME_MAX];
    } param;
};

#endif