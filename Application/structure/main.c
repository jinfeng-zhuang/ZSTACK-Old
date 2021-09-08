#include <zstack/zstack.h>

#include "main.h"

struct application app;

#define NAME_LEN_MAX    (32)

struct item {
    char var_name[NAME_LEN_MAX];
    char type_name[NAME_LEN_MAX];
    char type_alias_name[NAME_LEN_MAX];
    unsigned int *type; // point to int or custom type
    unsigned int size; // summarize
};

struct structure {
    int count;
    char type_name[NAME_LEN_MAX];
    char type_alias_name[NAME_LEN_MAX];
    struct item items[1];
};

static char* preprocess(char* str)
{
    // remove comment, space
}

int parse_tokens(char* str)
{
    
}

int structure_parse_name(char* str)
{

}

int main(int argc, char *argv[])
{
    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.input_file_size = file_size(app.param.input_filename);
    if (-1 == app.input_file_size) {
        warn("file not found\n");
        return -1;
    }

    app.input_file_content = (unsigned char*)malloc(app.input_file_size);

    file_load(app.param.input_filename, 0, app.input_file_content, app.input_file_size);



    return 0;
}
