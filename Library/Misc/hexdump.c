#pragma warning(disable:4996)

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zstack/log.h>
#include <zstack/types.h>
#include <zstack/misc.h>

int is_printable(unsigned char c)
{
    //if (((c >= 0x20) && (c < 0x7F)) || (c == '\n'))
    if ((c >= 0x20) && (c < 0x7F) && (c != '\n') && (c != '\r'))
        return TRUE;
    else
        return FALSE;
}

u8 printable(u8 c)
{
    if ((c >= 0x20) && (c < 0x7F))
        return c;
    else
        return '?';
}

int hexdump(void *start, unsigned int length, u32 flags)
{
    uint32_t i, j;
    uint32_t line_num;
    uint32_t last_bytes;
    char *hexdump_buffer;
    u32 hexdump_buffer_size;
    unsigned char *buffer = (unsigned char *)start;

    // step 1.
    line_num = length / 16;
    last_bytes = length % 16;
    if (last_bytes > 0)
        line_num = line_num + 1;

    // step 2.
    hexdump_buffer_size = line_num * 3 * 16; // [01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16]
    hexdump_buffer_size = hexdump_buffer_size + line_num * 1; // '\n'
    if (flags & HEXDUMP_ADDR) {
        hexdump_buffer_size = hexdump_buffer_size + line_num * 10; // [12345678: ]
    }
    if (flags & HEXDUMP_ASCII) {
        hexdump_buffer_size = hexdump_buffer_size + line_num * 16; // [ABCDEFGH87654321]
    }
    hexdump_buffer_size = hexdump_buffer_size + 1; // '\0'

    hexdump_buffer = (char *)malloc(hexdump_buffer_size);
    if (NULL == hexdump_buffer) {
        return -1;
    }
    else {
        memset(hexdump_buffer, 0, hexdump_buffer_size);
    }

    // step 3.
    for (i = 0; i < line_num; i++) {
        if ((i == line_num - 1) && (last_bytes))
            break;

        if (flags & HEXDUMP_ADDR) {
            sprintf(&hexdump_buffer[strlen(hexdump_buffer)], "%08x: ", (u32)(buffer + i * 16));
        }

        sprintf(&hexdump_buffer[strlen(hexdump_buffer)],
            "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x ",
            buffer[i * 16 + 0],
            buffer[i * 16 + 1],
            buffer[i * 16 + 2],
            buffer[i * 16 + 3],
            buffer[i * 16 + 4],
            buffer[i * 16 + 5],
            buffer[i * 16 + 6],
            buffer[i * 16 + 7],
            buffer[i * 16 + 8],
            buffer[i * 16 + 9],
            buffer[i * 16 + 10],
            buffer[i * 16 + 11],
            buffer[i * 16 + 12],
            buffer[i * 16 + 13],
            buffer[i * 16 + 14],
            buffer[i * 16 + 15]);


        if (flags & HEXDUMP_ASCII) {
            sprintf(&hexdump_buffer[strlen(hexdump_buffer)],
                "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
                printable(buffer[i * 16 + 0]),
                printable(buffer[i * 16 + 1]),
                printable(buffer[i * 16 + 2]),
                printable(buffer[i * 16 + 3]),
                printable(buffer[i * 16 + 4]),
                printable(buffer[i * 16 + 5]),
                printable(buffer[i * 16 + 6]),
                printable(buffer[i * 16 + 7]),
                printable(buffer[i * 16 + 8]),
                printable(buffer[i * 16 + 9]),
                printable(buffer[i * 16 + 10]),
                printable(buffer[i * 16 + 11]),
                printable(buffer[i * 16 + 12]),
                printable(buffer[i * 16 + 13]),
                printable(buffer[i * 16 + 14]),
                printable(buffer[i * 16 + 15]));
        }

        sprintf(&hexdump_buffer[strlen(hexdump_buffer)], "\n");
    }

    // step 4.
    if (last_bytes) {
        if (flags & HEXDUMP_ADDR) {
            sprintf(&hexdump_buffer[strlen(hexdump_buffer)], "%08x: ", (u32)(buffer + i * 16));
        }

        for (j = 0; j < last_bytes; j++) {
            sprintf(&hexdump_buffer[strlen(hexdump_buffer)], "%02x ", buffer[i * 16 + j]);
        }
        for (j = 0; j < 16 - last_bytes; j++) {
            sprintf(&hexdump_buffer[strlen(hexdump_buffer)], "   ");
        }

        if (flags & HEXDUMP_ASCII) {
            for (j = 0; j < last_bytes; j++) {
                sprintf(&hexdump_buffer[strlen(hexdump_buffer)], "%c", printable(buffer[i * 16 + j]));
            }
            for (j = 0; j < 16 - last_bytes; j++) {
                sprintf(&hexdump_buffer[strlen(hexdump_buffer)], " ");
            }
        }
    }

    // step 5.
    warn("%s\n", hexdump_buffer);
    
    // step 6.
    free(hexdump_buffer);
    
    return 0;
}
