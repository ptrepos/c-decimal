#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void get_binary_test(const char *text1)
{
	char strbuf[1000];
	int size;
	mg_decimal value1, value2;
	uint64_t high, low;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	
	mg_decimal_get_binary(&value1, /*out*/&high, /*out*/&low);
	
	mg_decimal_set_binary(/*out*/&value2, high, low);

	mg_assert(mg_decimal_to_string(&value2, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(text1, strbuf) == 0);
}

void decimal_get_binary_test()
{
	clock_t tm = clock();

	get_binary_test("123456789123456789");
	get_binary_test("-123456789123456789");
	get_binary_test("9999999999999999999999999999999999");
	get_binary_test("-9999999999999999999999999999999999");
	get_binary_test("1");
	get_binary_test("-1");
	get_binary_test("100000.9999999999999");
	get_binary_test("-100000.9999999999999");
	
	printf("TEST mg_decimal_get_binary(): OK\n");
}
