#include <stdio.h>

// TODO _fseeki64
unsigned int fsize(FILE *fp)
{
    int ret;
    long int size;

    if (NULL == fp)
        return -1;

    ret = fseek(fp, 0, SEEK_END);
    if (0 != ret)
        return -1;

    size = ftell(fp);
    if (-1 == size)
        return -1;

    ret = fseek(fp, 0, SEEK_SET);
    if (0 != ret)
        return -1;

    return size;
}
