#include <zstack/types.h>
#include <zstack/misc.h>

u64 BE_read_u64(u8* buffer) {
	u64 value;

	if (NULL == buffer) {
		return 0;
	}

	value = buffer[0] << 56 | buffer[1] << 48 | buffer[2] << 40 | buffer[3] << 32 |
		buffer[4] << 24 | buffer[5] << 16 | buffer[6] << 8 | buffer[7];

	return value;
}

u32 BE_read_u32(u8* buffer) {
	u32 value;

	if (NULL == buffer) {
		return 0;
	}

	value = (u32)(buffer[0]) << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];

	return value;
}

u16 BE_read_u16(u8* buffer) {
	u16 value;

	if (NULL == buffer) {
		return 0;
	}

	value = (u16)(buffer[0]) << 8 | buffer[1];

	return value;
}

u8 BE_read_u8(u8* buffer) {
	u8 value;

	if (NULL == buffer) {
		return 0;
	}

	value = (u8)buffer[0];

	return value;
}

u8 BE_read_u3(u8* buffer) {
	u8 value;

	if (NULL == buffer) {
		return 0;
	}

	value = buffer[0] >> 5;

	return value;
}

u8 BE_read_u5(u8* buffer) {
	u8 value;

	if (NULL == buffer) {
		return 0;
	}

	value = buffer[0] >> 3;

	return value;
}

u8 BE_read_u6(u8* buffer) {
	u8 value;

	if (NULL == buffer) {
		return 0;
	}

	value = buffer[0] >> 2;

	return value;
}
