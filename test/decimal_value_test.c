#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void value_test(mg_decimal value, const char *text)
{
	char strbuf[1000];
	int size;

	mg_assert(mg_decimal_to_string(&value, strbuf, 1000, &size) == 0);
	
	mg_assert(strcmp(strbuf, text) == 0);
}

void decimal_value_test()
{
	mg_decimal value;

	mg_decimal_zero(&value);
	value_test(value, "0");
	mg_decimal_one(&value);
	value_test(value, "1");
	mg_decimal_minus_one(&value);
	value_test(value, "-1");
	mg_decimal_max_value(&value);
	value_test(value, "999999999999999999999999999999999999");
	mg_decimal_min_value(&value);
	value_test(value, "-999999999999999999999999999999999999");

	printf("TEST mg_decimal values: OK\n");
}
