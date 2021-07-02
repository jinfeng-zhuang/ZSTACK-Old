#include <stdio.h>

unsigned int char_match(unsigned char *buffer, unsigned int length, unsigned char c)
{
    unsigned int i;
    unsigned int count = 0;

    if (NULL == buffer)
        return 0;

    for (i = 0; i < length; i++) {
        if (buffer[i] == c)
            count++;
    }

    return count;
}
