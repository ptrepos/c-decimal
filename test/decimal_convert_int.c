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
	mg_decimal value1;
	int value2;

	mg_assert(mg_decimal_value_of_int(value, &value1) == 0);

	mg_assert(mg_decimal_to_int(&value1, /*out*/&value2) == 0);

	mg_assert(value == value2);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(strbuf, ret) == 0);
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

	printf("TEST mg_decimal convert int methods: OK\n");
}
