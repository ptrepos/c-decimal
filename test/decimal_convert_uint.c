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

static void convert_overflow(const char *value)
{
	mg_decimal value1;
	unsigned int value3;

	mg_assert(mg_decimal_parse_string(value, &value1) == 0);
	mg_assert(mg_decimal_to_uint(&value1, /*out*/&value3) == MG_DECIMAL_ERROR_OVERFLOW);
}

void decimal_convert_uint_test()
{
	clock_t tm = clock();
	
	uint_convert_test(1000, "1000");
	uint_convert_test(999999999, "999999999");
	uint_convert_test(0, "0");
	uint_convert_test(2147483647, "2147483647");
	uint_convert_test(4294967295, "4294967295");

	convert_overflow("12345679812313214");
	convert_overflow("-12345679812313214");
	convert_overflow("-1");
	convert_overflow("4294967296");

	printf("TEST mg_decimal convert uint methods: OK\n");
}
