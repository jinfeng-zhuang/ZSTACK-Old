#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <zstack/log.h>

int file_save(const char* filename, unsigned char* buffer, unsigned int length)
{
    FILE* fp;
    unsigned int ret;

    if ((NULL == filename) || (NULL == buffer) || (length == 0)) {
        log_warn("param error\n");
        return -1;
    }

    fp = fopen(filename, "wb");
    if (NULL == fp) {
        log_warn("fopen %s failed\n", filename);
        return -1;
    }

    ret = fwrite(buffer, sizeof(unsigned char), length, fp);

    fflush(fp);

    if (ret != length) {
        log_warn("fwrite %s failed: %d != %d\n", filename, ret, length);
        return -1;
    }

    fclose(fp);

    debug("Save to '%s' done\n", filename);

    return 0;
}