#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <zstack/log.h>
#include <zstack/types.h>

int file_append(const char* filename, unsigned char* buffer, unsigned int length)
{
    FILE* fp;
    u32 ret;

    if ((NULL == filename) || (NULL == buffer) || (length == 0)) {
        warn("param error\n");
        return -1;
    }

    fp = fopen(filename, "ab");
    if (NULL == fp) {
        warn("fopen %s failed\n", filename);
        return -1;
    }

    ret = (u32)fwrite(buffer, sizeof(unsigned char), length, fp);

    fflush(fp);

    if (ret != length) {
        warn("fwrite %s failed: %d != %d\n", filename, ret, length);
        return -1;
    }

    fclose(fp);

    DEBUG("%s done\n", filename);

    return 0;
}