#include <zstack.h>

#define __func__ __FUNCTION__

unsigned char *file_load(const char* filename, unsigned int *size)
{
    int ret = 0;
    FILE* fp = NULL;
    unsigned int file_size = 0;
    unsigned int bytes_read = 0;
    unsigned char* buffer = NULL;

    fp = fopen(filename, "rb");
    if (NULL == fp) {
        log_info("%s: fopen_s failed %s\n", __func__, filename);
        goto END;
    }

    fseek(fp, 0, SEEK_END);

    file_size = ftell(fp);

    buffer = (unsigned char*)malloc(file_size);
    if (NULL == buffer) {
        printf("%s: malloc failed %d\n", __func__, file_size);
        goto END;
    }

    fseek(fp, 0, SEEK_SET);

    bytes_read = fread(buffer, sizeof(unsigned char), file_size, fp);
    if (bytes_read != file_size) {
        printf("%s: fread failed %d\n", __func__, bytes_read);
        goto END;
    }

END:
    if (fp)
        fclose(fp);

    *size = file_size;

    return buffer;
}
