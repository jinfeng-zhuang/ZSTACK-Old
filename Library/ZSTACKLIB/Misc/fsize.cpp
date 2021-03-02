#include <stdio.h>
#include <stdint.h>

// TODO _fseeki64
int64_t fsize(const char *filename)
{
    int ret;
    int64_t size;
    FILE *fp;

    fp = fopen(filename, "rb");

    if (NULL == fp)
        return -1;

    ret = _fseeki64(fp, 0, SEEK_END);
    if (0 != ret)
        return -1;

    size = _ftelli64(fp);
    if (-1 == size)
        return -1;

    ret = _fseeki64(fp, 0, SEEK_SET);
    if (0 != ret)
        return -1;

    fclose(fp);

    return size;
}
