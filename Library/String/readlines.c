#include <zstack/types.h>

// TBD
// return current line
// usage: while (s = readlines(buffer, length, &offset, s))
u8 *readlines(u8 * buffer, u32 length, u32 *offset, u8 *cur)
{
	if ((NULL == buffer) || (NULL == offset) || (0 == length)) {
		return NULL;
	}

	return NULL;
}
