#include <stdio.h>

unsigned char I420_Y(int chroma_x, int chroma_y, int stride, int height, unsigned char* frame_buffer)
{
    unsigned char value;

    value = frame_buffer[stride * chroma_y + chroma_x];

    return value;
}

unsigned char I420_U(int chroma_x, int chroma_y, int stride, int height, unsigned char* frame_buffer)
{
    int x = chroma_x >> 1;
    int y = chroma_y >> 1;

    unsigned char* buffer = &frame_buffer[stride * height];

    unsigned char value;

    value = buffer[(stride >> 1) * y + x];

    return value;
}

unsigned char I420_V(int chroma_x, int chroma_y, int stride, int height, unsigned char* frame_buffer)
{
    int x = chroma_x >> 1;
    int y = chroma_y >> 1;

    unsigned char* buffer = &frame_buffer[stride * height];

    unsigned char value;

    value = buffer[(stride >> 1) * (height >> 1) + (stride >> 1) * y + x];

    return value;
}

unsigned char NV12_Y(int chroma_x, int chroma_y, int stride, int height, unsigned char* frame_buffer)
{
    unsigned char value;

    value = frame_buffer[stride * chroma_y + chroma_x];

    return value;
}

unsigned char NV12_U(int chroma_x, int chroma_y, int stride, int height, unsigned char* frame_buffer)
{
    int x = chroma_x >> 1;
    int y = chroma_y >> 1;

    unsigned char* buffer = &frame_buffer[stride * height];

    unsigned char value;

    value = buffer[(stride) * y + x * 2 + 0];

    return value;
}

unsigned char NV12_V(int chroma_x, int chroma_y, int stride, int height, unsigned char* frame_buffer)
{
    int x = chroma_x >> 1;
    int y = chroma_y >> 1;

    unsigned char* buffer = &frame_buffer[stride * height];

    unsigned char value;

    value = buffer[(stride) * y + x * 2 + 1];

    return value;
}

unsigned char Tile_Y(int x, int y, int w, int h, unsigned char* buffer)
{
    unsigned char value;
    int x1, y1;

    w = w * 4;
    h = h / 4;

    x1 = (x / 4) * 16 + (y % 4) * 4 + (x % 4); // 第 M 区间 X 小区间 偏移 Z
    y1 = y / 4;
    value = buffer[y1 * w + x1];

    return value;
}

unsigned char Tile_U(int x, int y, int w, int h, unsigned char* frame_buffer)
{
    unsigned char value;
    int x1, y1;
    unsigned char* buffer = &frame_buffer[w * h];

    w = w * 4;
    h = h / 4;

    x1 = (x / 4) * 16 + (y % 4) * 4 + (x % 4); // 第 M 区间 X 小区间 偏移 Z
    y1 = y / 4;

    x1 >>= 1;
    y1 >>= 1;

    value = buffer[y1 * (w) + x1 * 2 + 0];

    return value;
}

unsigned char Tile_V(int x, int y, int w, int h, unsigned char* frame_buffer)
{
    unsigned char value;
    int x1, y1;
    unsigned char* buffer = &frame_buffer[w * h];

    w = w * 4;
    h = h / 4;

    x1 = (x / 4) * 16 + (y % 4) * 4 + (x % 4); // 第 M 区间 X 小区间 偏移 Z
    y1 = y / 4;

    x1 >>= 1;
    y1 >>= 1;

    value = buffer[y1 * (w) + x1 * 2 + 1];

    return value;
}

int flag;
extern int total_len;

static unsigned char Buffer_10bit(unsigned char* buffer, unsigned int index)
{
    int bit_index = index * 10 / 8;
    int bit_offset = index * 10 % 8;
    unsigned char mask1 = ~(1 << bit_offset - 1);
    unsigned char mask2 = 1 << (10 - 8 + bit_offset) - 1;

    unsigned char byte1 = (buffer[bit_index] & mask1) >> bit_offset;
    unsigned char byte2 = buffer[bit_index + 1] & mask2;

    unsigned short value = (unsigned short)byte2 << 8 | byte1;

    unsigned char result = (unsigned char)(value >> 2);

    return result;
}

unsigned char Tile10_Y(int x, int y, int w, int h, unsigned char* buffer)
{
    unsigned char value;
    int x1, y1;

    w = w * 4;
    h = h / 4;

    x1 = (x / 4) * 16 + (y % 4) * 4 + (x % 4); // 第 M 区间 X 小区间 偏移 Z
    y1 = y / 4;

    if (y >= 880)
        flag = 1;

    value = Buffer_10bit(buffer, y1 * w + x1);

    return value;
}

unsigned char Tile10_U(int x, int y, int w, int h, unsigned char* frame_buffer)
{
    unsigned char value;
    int x1, y1;
    unsigned char* buffer = &frame_buffer[(w * h) * 10 / 8];

    w = w * 4;
    h = h / 4;

    x1 = (x / 4) * 16 + (y % 4) * 4 + (x % 4); // 第 M 区间 X 小区间 偏移 Z
    y1 = y / 4;

    x1 >>= 1;
    y1 >>= 1;

    value = Buffer_10bit(buffer, y1 * (w)+x1 * 2 + 0);

    return value;
}

unsigned char Tile10_V(int x, int y, int w, int h, unsigned char* frame_buffer)
{
    unsigned char value;
    int x1, y1;
    unsigned char* buffer = &frame_buffer[(w * h) * 10 / 8];

    w = w * 4;
    h = h / 4;

    x1 = (x / 4) * 16 + (y % 4) * 4 + (x % 4); // 第 M 区间 X 小区间 偏移 Z
    y1 = y / 4;

    x1 >>= 1;
    y1 >>= 1;

    value = Buffer_10bit(buffer, y1 * (w)+x1 * 2 + 1);

    return value;
}
