#include <zstack/types.h>

u16 crc16(u8* buffer, u32 length)
{
	u16 wCRCin = 0x0000;
	u16 wCPoly = 0x1021;
	u8  wChar  = 0;
	u8  i;

	while (length--)
	{
		wChar = *(buffer++);
		wCRCin ^= (wChar << 8);
		for (i = 0; i < 8; i++)
		{
			if (wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
}
