#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void uint64_convert_test(long long value, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal_t value1;
	unsigned long long value2;

	mg_assert(mg_decimal_value_of_ulong_long(value, &value1) == 0);

	mg_assert(mg_decimal_to_ulong_long(&value1, /*out*/&value2) == 0);

	mg_assert(value == value2);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(strbuf, ret) == 0);
}

static void convert_overflow(const char *value)
{
	mg_decimal_t value1;
	unsigned long long value3;

	mg_assert(mg_decimal_parse_string(value, &value1) == 0);
	mg_assert(mg_decimal_to_ulong_long(&value1, /*out*/&value3) == mgE_OVERFLOW);
}

void decimal_convert_uint64_test()
{
	clock_t tm = clock();
	
	uint64_convert_test(1000ULL, "1000");
	uint64_convert_test(9999999999999ULL, "9999999999999");
	uint64_convert_test(999999999999999999ULL, "999999999999999999");
	uint64_convert_test(425415311ULL, "425415311");
	uint64_convert_test(9223372036854775807ULL, "9223372036854775807");
	uint64_convert_test(0ULL, "0");
	uint64_convert_test(18446744073709551615ULL, "18446744073709551615");

	convert_overflow("123456789123456789123456789");
	convert_overflow("-123456789123456789123456789");
	convert_overflow("-1");
	convert_overflow("18446744073709551616");
	
	printf("TEST mg_decimal convert uint64 methods: OK\n");
}
