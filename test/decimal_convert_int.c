#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void int_convert_test(int value, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal_t value1;
	int value2;

	mg_assert(mg_decimal_value_of_int(value, &value1) == 0);

	mg_assert(mg_decimal_to_int(&value1, /*out*/&value2) == 0);

	mg_assert(value == value2);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(strbuf, ret) == 0);
}

static void convert(const char *ret, int value)
{
	mg_decimal_t value1;
	int value3;

	mg_assert(mg_decimal_parse_string(ret, &value1) == 0);

	mg_assert(mg_decimal_to_int(&value1, /*out*/&value3) == 0);

	mg_assert(value == value3);
}

static void convert_overflow(const char *ret)
{
	mg_decimal_t value1;
	int value3;

	mg_assert(mg_decimal_parse_string(ret, &value1) == 0);
	mg_assert(mg_decimal_to_int(&value1, /*out*/&value3) == mgE_OVERFLOW);
}

void decimal_convert_int_test()
{
	clock_t tm = clock();
	
	int_convert_test(1000, "1000");
	int_convert_test(-1000, "-1000");
	int_convert_test(-999999999, "-999999999");
	int_convert_test(0, "0");
	int_convert_test(-2147483647, "-2147483647");
	int_convert_test(-2147483647-1, "-2147483648");
	int_convert_test(2147483647, "2147483647");

	convert("123456", 123456);
	convert("99999999", 99999999);
	convert("-123456", -123456);
	convert("-99999999", -99999999);
	convert("-123456.123456", -123456);
	convert("-99999999.123456", -99999999);
	convert("123456.123456", 123456);
	convert("99999999.123456", 99999999);

	convert_overflow("123456789123");
	convert_overflow("-123456789123");
	convert_overflow("2147483648");
	convert_overflow("-2147483649");

	printf("TEST mg_decimal convert int methods: OK\n");
}
