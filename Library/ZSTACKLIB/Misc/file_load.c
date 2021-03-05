#define _CRT_SECURE_NO_WARNINGS

#include <zstack.h>

#define __func__ __FUNCTION__

unsigned char *file_load(const char* filename, unsigned int *size)
{
    int ret = 0;
    FILE* fp = NULL;
    uint64_t file_size = 0;
    unsigned int bytes_read = 0;
    unsigned char* buffer = NULL;

    fp = fopen(filename, "rb");
    if (NULL == fp) {
        log_info("%s: fopen_s failed %s\n", __func__, filename);
        goto END;
    }

    _fseeki64(fp, 0, SEEK_END);

    file_size = _ftelli64(fp);

    if (file_size > (1<<30)) {
        warn("file size > 1GB, use file_load_ex instead\n");
        return NULL;
    }

    buffer = (unsigned char*)malloc((unsigned int)file_size);
    if (NULL == buffer) {
        printf("%s: malloc failed %d\n", __func__, file_size);
        goto END;
    }

    _fseeki64(fp, 0, SEEK_SET);

    bytes_read = fread(buffer, sizeof(unsigned char), (unsigned int)file_size, fp);
    if (bytes_read != file_size) {
        printf("%s: fread failed %d\n", __func__, bytes_read);
        goto END;
    }

END:
    if (fp)
        fclose(fp);

    *size = (unsigned int)file_size;

    return buffer;
}

unsigned char *file_load_ex(const char* filename, unsigned long long offset, unsigned int size)
{
    int ret = 0;
    FILE* fp = NULL;
    unsigned int bytes_read = 0;
    unsigned char* buffer = NULL;

    info("Offset [%llx]\n", offset);

    fp = fopen(filename, "rb");
    if (NULL == fp) {
        log_info("%s: fopen_s failed %s\n", __func__, filename);
        goto END;
    }

    _fseeki64(fp, offset, SEEK_SET);

    buffer = (unsigned char*)malloc(size);
    if (NULL == buffer) {
        printf("%s: malloc failed %d\n", __func__, size);
        goto END;
    }

    memset(buffer, 0, size);

    bytes_read = fread(buffer, sizeof(unsigned char), size, fp);
    if (bytes_read != size) {
        printf("%s: fread failed %d\n", __func__, bytes_read);
        goto END;
    }

END:
    if (fp)
        fclose(fp);

    return buffer;
}
