#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <zstack/log.h>

int file_append(const char* filename, unsigned char* buffer, unsigned int length)
{
    FILE* fp;
    unsigned int ret;

    if ((NULL == filename) || (NULL == buffer) || (length == 0)) {
        log(LOG_WARNING, "param error\n");
        return -1;
    }

    fp = fopen(filename, "a");
    if (NULL == fp) {
        log(LOG_WARNING, "fopen %s failed\n", filename);
        return -1;
    }

    ret = fwrite(buffer, sizeof(unsigned char), length, fp);

    if (ret != length) {
        log(LOG_WARNING, "fwrite %s failed: %d != %d\n", filename, ret, length);
        return -1;
    }

    fclose(fp);

    log(LOG_DEBUG, "%s done\n", filename);

    return 0;
}