#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <zstack/types.h>
#include <zstack/log.h>

u64 file_size(const char *filename)
{
    int ret;
    u64 size;
    FILE *fp;

    fp = fopen(filename, "rb");
    if (NULL == fp) {
        info("open %s failed\n", filename);
        return -1;
    }

    ret = _fseeki64(fp, 0, SEEK_END);
    if (0 != ret) {
        printf("seek end failed\n");
        return -1;
    }

    size = _ftelli64(fp);
    if (-1 == size) {
        printf("ftell set failed\n");
        return -1;
    }

    ret = _fseeki64(fp, 0, SEEK_SET);
    if (0 != ret) {
        printf("seek set failed\n");
        return -1;
    }

    fclose(fp);

    return size;
}
