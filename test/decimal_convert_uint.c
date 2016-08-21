#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void uint_convert_test(unsigned int value, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal value1;
	int value2;

	mg_assert(mg_decimal_value_of_uint(value, &value1) == 0);

	mg_assert(mg_decimal_to_uint(&value1, /*out*/&value2) == 0);

	mg_assert(value == value2);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(strbuf, ret) == 0);
}

void decimal_convert_uint_test()
{
	clock_t tm = clock();
	
	uint_convert_test(1000, "1000");
	uint_convert_test(999999999, "999999999");
	uint_convert_test(0, "0");
	uint_convert_test(2147483647, "2147483647");
	uint_convert_test(4294967295, "4294967295");

	printf("TEST mg_decimal convert uint methods: OK\n");
}
