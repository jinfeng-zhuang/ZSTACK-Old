#include <zstack/yuv.h>
#include <zstack/types.h>

static unsigned int FrameSize(int width, int height)
{
	return width * height * 3;
}

static unsigned short GetY(unsigned char* buffer, int width, int height, int x, int y)
{
	u8 value;

	if (NULL == buffer)
		return 0;

	value = buffer[width * y + x];

	return value;
}

static unsigned short GetU(unsigned char* buffer, int width, int height, int x, int y)
{
	u8 value;

	if (NULL == buffer)
		return 0;

	value = buffer[width * height + width * y + x];

	return value;
}

static unsigned short GetV(unsigned char* buffer, int width, int height, int x, int y)
{
	u8 value;

	if (NULL == buffer)
		return 0;

	value = buffer[width * height + width * height + width * y + x];

	return value;
}

static void SetY(unsigned char* buffer, int width, int height, int x, int y, unsigned short value)
{
	buffer[width * y + x] = value;
}

static void SetU(unsigned char* buffer, int width, int height, int x, int y, unsigned short u)
{
	buffer[width * height + width * y + x] = u;
}

static void SetV(unsigned char* buffer, int width, int height, int x, int y, unsigned short v)
{
	buffer[width * height + width * height + width * y + x] = v;
}

struct YUV_Wrapper yuv_wrapper_yuv444 = {
	FrameSize,
	GetY, GetU, GetV,
	SetY, SetU, SetV
};