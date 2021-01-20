#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <zstack/list.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char input_filename[FILENAME_MAX];
        char output_filename[FILENAME_MAX];
    } param;

    unsigned char *input_file_content;
    unsigned int   input_file_size;

    unsigned char *output_file_content;
    unsigned int   output_file_size;
};

#define HS_ATTR_MAX 128

enum hs_attr_e {
    HS_ATTR_TYPE,
    HS_ATTR_PRIO,
    HS_ATTR_BLOOD,
    HS_ATTR_ATTACK,
    HS_ATTR_COST,
};

enum hs_type_e {
    HS_TYPE_RETINUE,
    HS_TYPE_MAGIC,
};

enum hs_attr_operation_e {
};

enum hs_prio_e {
    HS_PRIO_COMMON,
    HS_PRIO_MAGIC_HIGH,
    HS_PRIO_MAGIC_COMMON,
    HS_PRIO_RETINUE_HIGH,
    HS_PRIO_RETINUE_COMMON,
};

struct hs_attr {
    int id;
    int value; // -1 means card.attr.value - 1
    struct list_head list;
};

struct hs_event {
    struct list_head match;
    struct list_head operation;
};

typedef int (*hs_card_callback)(struct hs_event *evt);

struct hs_card {
    wchar_t name[128];

    int attr[HS_ATTR_MAX];

    struct list_head history;

    struct list_head list;

    hs_card_callback cb;
};

#ifdef __cplusplus
}
#endif

#endif