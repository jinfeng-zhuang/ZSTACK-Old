#include <string.h>

int split(char *dst, int dst_size, char *src, int src_size, char flag, int *offset)
{
	unsigned int i;
	unsigned int j;

	if ((NULL == dst) || (NULL == src) || (NULL == offset))
		return -1;

	if ((unsigned int)*offset >= strlen(src))
		return -1;

	memset(dst, 0, dst_size);

	for (i = *offset, j = 0; i < strlen(src); i++) {
		if (src[i] == flag) {
			i++; // SKIP the flag
			break;
		}
		else {
			dst[j] = src[i];
			j++;

			if (j >= (unsigned int)dst_size)
				break;
		}
	}

	*offset = i;

	return 0;
}
