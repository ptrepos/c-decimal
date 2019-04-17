#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void convert_test(long long value, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal_t value1;
	long long value2;

	mg_assert(mg_decimal_value_of_long_long(value, &value1) == 0);

	mg_assert(mg_decimal_to_long_long(&value1, /*out*/&value2) == 0);

	mg_assert(value == value2);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(strbuf, ret) == 0);
}

static void convert_overflow(const char *value)
{
	mg_decimal_t value1;
	long long value3;

	mg_assert(mg_decimal_parse_string(value, &value1) == 0);
	mg_assert(mg_decimal_to_long_long(&value1, /*out*/&value3) == mgE_OVERFLOW);
}

void decimal_convert_long_long_test()
{
	clock_t tm = clock();
	
	convert_test(1000, "1000");
	convert_test(-1000, "-1000");
	convert_test(-9999999999999, "-9999999999999");
	convert_test(-999999999999999999, "-999999999999999999");
	convert_test(-425415311, "-425415311");
	convert_test(9223372036854775807, "9223372036854775807");
	convert_test(-9223372036854775807LL - 1LL, "-9223372036854775808");
	convert_test(0, "0");

	convert_overflow("12345678912345678912345961");
	convert_overflow("-12345678912345678912345961");
	convert_overflow("9223372036854775808");
	convert_overflow("-9223372036854775809");

	{
		mg_decimal_t value;
		mg_assert(mg_decimal_parse_string("1234567891234156498715634865156465151654152165453132416854114", &value) == mgE_OVERFLOW);
	}

	printf("TEST mg_decimal convert long long methods: OK\n");
}
