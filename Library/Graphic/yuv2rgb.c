#include <zstack/types.h>

#if 0
u32 yuv2rgba(u32 yuv)
{
	u32 result = 0;
	u8 Y, U, V;
	u8 R, G, B;

	Y = yuv & 0xFF;
	U = (yuv >> 8) & 0xFF;
	V = (yuv >> 16) & 0xFF;

	Y -= 16;
	U -= 128;
	V -= 128;
	R = (u8)(1.164 * Y + 1.596 * V);
	G = (u8)(1.164 * Y - 0.392 * U - 0.813 * V);
	B = (u8)(1.164 * Y + 2.017 * U);

	R = (R > 255) ? 255 : R;
	G = (G > 255) ? 255 : G;
	B = (B > 255) ? 255 : B;

	R = (R < 0) ? 0 : R;
	G = (G < 0) ? 0 : G;
	B = (B < 0) ? 0 : B;

	result = ((u32)B << 16) | (G << 8) | (R << 0);

	return result;
}
#else
u32 yuv2rgba(u32 yuv)
{
	u8 r, g, b;
	u8 y, u, v;

	y = yuv & 0xFF;
	u = yuv >> 8;
	v = yuv >> 16;

	r = 1.0 * y + 0 + 1.402 * (v - 128);
	g = 1.0 * y - 0.34413 * (u - 128) - 0.71414 * (v - 128);
	b = 1.0 * y + 1.772 * (u - 128) + 0;

	r = (r > 255) ? 255 : r;
	g = (g > 255) ? 255 : g;
	b = (b > 255) ? 255 : b;

	r = (r < 0) ? 0 : r;
	g = (g < 0) ? 0 : g;
	b = (b < 0) ? 0 : b;

	return b | g << 8 | r << 16;
}
#endif