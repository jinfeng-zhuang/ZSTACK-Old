#include <zstack/types.h>

u32 rgba2yuv(u32 rgba)
{
	u8 Y, U, V;
	u8 R, G, B;

	R = rgba & 0xFF;
	G = (rgba >> 8) & 0xFF;
	B = (rgba >> 16) & 0xFF;

	Y = (u8)(0.257 * R + 0.504 * G + 0.098 * B + 16);
	U = (u8)(-0.148 * R - 0.291 * G + 0.439 * B + 128);
	V = (u8)(0.439 * R - 0.368 * G - 0.071 * B + 128);

	return (u32)(V << 16) | (U << 8) | (Y << 0);
}
