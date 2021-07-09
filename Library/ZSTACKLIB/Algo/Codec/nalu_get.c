#include <stdio.h>
#include <zstack/types.h>

int nalu_get(u8* buffer, u32 length, u32 *start, u32* size)
{
	u32 i;
	u32 nalu_start_flag;
	u32 nalu_start_offset;
	u32 _start;
	u32 _start2;

	nalu_start_flag = 0;
	nalu_start_offset = 0;
	_start = *start + *size;

	for (i = _start; i < length; i++) {
		if ((0 == buffer[i]) && (0 == buffer[i + 1]) && (0 == buffer[i + 2]) && (1 == buffer[i + 3])) {
			nalu_start_flag = TRUE;
			_start = i;
			i = i + 4;
			break;
		}
		else if ((0 == buffer[i]) && (0 == buffer[i + 1]) && (1 == buffer[i + 2])) {
			nalu_start_flag = TRUE;
			_start = i;
			i = i + 3;
			break;
		}
	}

	if (FALSE == nalu_start_flag)
		return FALSE;
	nalu_start_flag = FALSE;

	for (; i < length; i++) {
		if ((0 == buffer[i]) && (0 == buffer[i + 1]) && (0 == buffer[i + 2]) && (1 == buffer[i + 3])) {
			nalu_start_flag = TRUE;
			_start2 = i;
			break;
		}
		else if ((0 == buffer[i]) && (0 == buffer[i + 1]) && (1 == buffer[i + 2])) {
			nalu_start_flag = TRUE;
			_start2 = i;
			break;
		}
	}

	if (FALSE == nalu_start_flag)
		return FALSE;

	*start = _start;
	*size = _start2 - _start;

	return TRUE;
}
