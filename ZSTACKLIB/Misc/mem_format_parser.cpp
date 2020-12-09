#define _CRT_SECURE_NO_WARNINGS

#include <zstack.h>

#include <assert.h>

/*
 * - binary:	load directly
 * - hexdump:	11 22 33 44
 * - simple:	4:1 2 3 4, 2:1 2 3
 */

// TODO: handle "1 4: 5 56" failed
int mem_format_parser_simple(const char *input, unsigned char *output)
{
	char level_1[1024];
	char level_2[1024];
	char str_size[64];
	char level_3[64];

	int level_1_index = 0;
	int level_3_index = 0;

	unsigned int output_offset = 0;
	int value_size;
	unsigned int value;

	if ((NULL == input) || (NULL == output))
		return -1;
	
	while (-1 != split(level_1, 1024, (char *)input, strlen(input), ',', &level_1_index)) {

		trim(level_1);
		if (0 == strlen(level_1))
			continue;

		str_size[0] = '\0';
		level_2[0] = '\0';

		sscanf(level_1, "%[^:]:%[^:]", str_size, level_2);
		trim(str_size);
		trim(level_2);

		//printf(">>> %s => %s\n", str_size, level_2);

		trim(level_2);
		if ((0 == strlen(level_2)) || (0 == strlen(str_size)))
			continue;

		value_size = atoi(str_size);

		//printf("value size %d string = |%s|\n", value_size, level_2);

		level_3_index = 0;
		while (-1 != split(level_3, 64, (char *)level_2, strlen(level_2), ' ', &level_3_index)) {
			trim(level_3);
			if (0 == strlen(level_3))
				continue;

			//printf("value string level_1 = |%s|\n", level_3);
			if ((strlen(level_3) > 2) && ('0' == level_3[0]) && (('x' == level_3[1]) || ('X' == level_3[1]))) {
				sscanf(level_3, "%x", &value);
			}
			else {
				sscanf(level_3, "%d", &value);
			}

			if (value_size == 4) {
				assert(output_offset % 4 == 0);
				*((unsigned int *)&output[output_offset]) = (unsigned int)value;
				output_offset += 4;
			}
			else if (value_size == 2) {
				assert(output_offset % 2 == 0);
				*((unsigned short *)&output[output_offset]) = (unsigned short)value;
				output_offset += 2;
			}
			else {
				*((unsigned char *)&output[output_offset]) = (unsigned char)value;
				output_offset++;
			}

			//printf("value = %x\n", value);
		}
	}

	return 0;
}

void mem_format_parser_test(void)
{
	unsigned char output[64];

	memset(output, 0xcc, 64);

	mem_format_parser_simple("   4:1 0x20 0x300 4  , 2  :  1    65535 0x22   ,    1 4: 5 56", output);

	hexdump(output, 64);
}
