#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    unsigned char *file_content;
    struct ivf_file_header *file_header;
    unsigned int i;
    unsigned long long timestamp;
    unsigned int length;
    unsigned int file_size;
    unsigned int prev_offset = 0;
    unsigned int prev_size = 0;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    // we should assume the vp9 file is very large, 1G? so file_load can't work for this. should change to small_file_load

    file_content = file_load(app.param.filename, &file_size);
    if (NULL == file_content) {
        log_warn("file open failed: %s\n", app.param.filename);
        return -1;
    }

    file_header = (struct ivf_file_header *)file_content;

    for (i = 0x20; i < file_size;) {
        length = *((unsigned int *)&file_content[i]);
        timestamp = *((unsigned long long *)&file_content[i + 4]);
        //printf("%08x: size %d: %08llx\n", i, length, timestamp);
		log_info("size %08x, offset %08x, expected %08x\n", length, i, prev_offset + prev_size + 12);
        prev_offset = i;
        prev_size = length;
        i += 12;
        i += length;
    }

    return 0;
}
