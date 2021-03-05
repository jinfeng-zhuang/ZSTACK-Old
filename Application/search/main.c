#include <zstack.h>

#include "main.h"

struct application app;

static int search(unsigned char *src, unsigned int srclen, unsigned char *pattern, unsigned int pattern_len, unsigned int *offset, unsigned int *size)
{
    unsigned int i, j;
    unsigned int max_count;
    unsigned int max_offset;
    unsigned int flag = 0;

    max_count = *size;
    max_offset = 0;

    for (i = 0; i < srclen; i++) {
        for (j = 0; j < pattern_len; j++) {
            if ((i + j) >= srclen)
                break;

            if (pattern[j] != src[i + j]) {
                if (max_count < j) {
                    max_count = j;
                    max_offset = i;

                    flag = 1;
                }

                break;
            }
        }
    }

    *offset = max_offset;
    *size = max_count;

    return flag;
}

int main(int argc, char *argv[])
{
    unsigned int i;
    unsigned int max_offset;
    unsigned int max_count;
    unsigned int region_count = 10;
    unsigned int patter_size;
    unsigned int patter_base;
    unsigned int step;
    unsigned int region;
    unsigned int region_selected;
    int ret;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.input_file_content = file_load(app.param.filename, &app.input_file_size);
    if (NULL == app.input_file_content)
        return -1;

    app.pattern_content = file_load(app.param.pattern, &app.pattern_size);
    if (NULL == app.pattern_content)
        return -1;

    // 跳着搜索，然后在最大值的地方来回搜索，确定最大值
    patter_size = app.pattern_size / region_count;
    patter_base = 0;

    max_count = 0;
    step = 100*1000;
    patter_base = 0;
    patter_size = app.pattern_size;
    region_selected = 0;

    while (step > 1) {
        info("Pattern: START %d Size %d Step = %d\n", patter_base, patter_size, step);

        for (i = patter_base; i < patter_size; i += step) {
            if ((app.pattern_size - i) <= max_count)
                break;

            ret = search(app.input_file_content, app.input_file_size, &app.pattern_content[i], patter_size - i, &max_offset, &max_count);

            if (ret) {
                region_selected = i;
                info(">> Pattern[%d] = Source[%d], Count %d bytes\n", region_selected, max_offset, max_count);
            }
        }

        // Find the optimized region

        if (region_selected > step)
            patter_base = region_selected - step;
        patter_size = app.pattern_size - patter_base;
        step = step >> 1;
    }

    info("END: Pattern[%d] = Source[%d], Count %d bytes\n", region_selected, max_offset, max_count);

    return 0;
}
