#define _CRT_SECURE_NO_WARNINGS

#include <zstack/types.h>
#include <zstack/log.h>
#include <zstack/file.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

u32 file_load(const char* filename, u64 offset, u8 *buffer, u32 length)
{
    int ret = 0;
    FILE* fp = NULL;
    unsigned int bytes_read = -1;

    if (NULL == buffer) {
        warn("buffer is NULL\n");
        goto END;
    }

    fp = fopen(filename, "rb");
    if (NULL == fp) {
        warn("fopen failed %s\n", filename);
        goto END;
    }

    _fseeki64(fp, offset, SEEK_SET);

    bytes_read = (u32)fread(buffer, sizeof(unsigned char), length, fp);

    if (bytes_read != length) {
        if (0 != ferror(fp)) {
            bytes_read = -1;

            warn("errno = %d\n", errno);
        }
    }

END:
    if (fp)
        fclose(fp);

    return bytes_read;
}

unsigned char *file_load_simple(const char* filename, u64 offset, u32 *length)
{
    int ret = 0;
    FILE* fp = NULL;
    unsigned int bytes_read = -1;
    u8* buffer = NULL;
    long long size = 0;
    u32 len = 0;

    if ((NULL == length) || (NULL == filename)) {
        warn("param shouldn't be NULL\n");
        goto END;
    }

    // check parameters
    size = file_size(filename);

    if (offset > size) {
        warn("offset > file size, set to file size\n");
        offset = size;
    }

    len = size - offset;

    // malloc buffer to read
    buffer = malloc(len);
    if (NULL == buffer) {
        warn("malloc failed\n");
        goto END;
    }

    bytes_read = file_load(filename, offset, buffer, len);

    if (bytes_read != len) {
        warn("fread @%d failed: errno = %d, modified request %d, but read back %d\n",
            offset, errno, len, bytes_read);

        free(buffer);
        buffer = NULL;
    }
END:

    info("file size = %d, bytes2read = %d, bytesread = %d\n", size, len, bytes_read);

    if (length) {
        *length = len;
    }

    return buffer;
}
