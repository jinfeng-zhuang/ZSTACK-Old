#include <string.h>
#include <stdio.h>

// TEST CASE
// ""
// "X"
// " "
// "      "
// "   X   "
// "  Hello   "

/*
 * Attention: str should be in array!!!
 */
char *trim(char *str)
{
	int len;
	int i;
	int start;
	int end;

	if (NULL == str)
		return NULL;

	len = strlen(str);

	if (0 == len)
		return str;

	i = 0;
	while ((str[i++] == ' ') && (i < len));
	start = i - 1;

	if ((str[i-1] == ' ') && (i == len)) {
		start = 0x7FFFFFFF;
	}
	
	i = len - 1;
	while ((str[i--] == ' ') && (i >= 0));
	end = i + 1;
	
	//printf("start = %d, end = %d\n", start, end);

	if (start > end) {
		str[0] = '\0';
		len = 0;
	}
	else {
		memcpy(str, &str[start], end - start + 1);
		str[end - start + 1] = '\0';
		len = strlen(str);
	}

	return str;
}

int trim_test(void)
{
	char value0[] = "";
	printf("|%s|\n", trim(value0));

	char value1[] = "X";
	printf("|%s|\n", trim(value1));

	char value2[] = " ";
	printf("|%s|\n", trim(value2));

	char value3[] = "  ";
	printf("|%s|\n", trim(value3));
	return 0;
}