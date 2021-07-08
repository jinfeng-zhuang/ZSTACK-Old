#include <zstack/zstack.h>

unsigned int replace_char(unsigned char *buffer, unsigned int length, unsigned char origin, unsigned char target)
{
    unsigned int i;
    unsigned int count;

    if (buffer == NULL)
        return 0;

    count = 0;

    for (i = 0; i < length; i++) {
        if (buffer[i] == origin) {
            buffer[i] = target;
            count++;
        }
    }

    return count;
}
