#include <stdio.h>

unsigned int count_char(unsigned char *buffer, unsigned int length, unsigned char c)
{
    unsigned int i;
    unsigned int count;

    if (NULL == buffer)
        return 0;

    count = 0;

    for (i = 0; i < length; i++) {
        if (buffer[i] == c)
            count++;
    }

    return count;
}
