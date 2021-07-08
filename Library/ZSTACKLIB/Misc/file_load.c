#define _CRT_SECURE_NO_WARNINGS

#include <zstack/types.h>
#include <zstack/log.h>
#include <stdio.h>

u32 file_load(const char* filename, u64 offset, u8 *buffer, u32 length)
{
    int ret = 0;
    FILE* fp = NULL;
    unsigned int bytes_read = 0;

    fp = fopen(filename, "rb");
    if (NULL == fp) {
        warn("fopen failed %s\n", filename);
        bytes_read = -1;
        goto END;
    }

    _fseeki64(fp, offset, SEEK_SET);

    bytes_read = (u32)fread(buffer, sizeof(unsigned char), length, fp);
    if (bytes_read != length) {
        warn("%s: fread failed %d\n", __func__, bytes_read);
        goto END;
    }

END:
    if (fp)
        fclose(fp);

    return bytes_read;
}
