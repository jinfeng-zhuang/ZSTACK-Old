#include <zstack/types.h>

u32 yuv2rgba(u32 yuv)
{
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

	return (u32)(B << 16) | (G << 8) | (R << 0);
}
