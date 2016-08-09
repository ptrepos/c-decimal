#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void modulus_test(const char *text1, const char *text2, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal value1, value2, value3;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	mg_assert(mg_decimal_parse_string(text2, &value2) == 0);

	mg_assert(mg_decimal_modulus(&value1, &value2, /*out*/&value3) == 0);

	mg_assert(mg_decimal_to_string(&value3, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(ret, strbuf) == 0);
}

void decimal_modulus_test()
{
	clock_t tm = clock();
	
	modulus_test("100000000000", "3", "1");
	modulus_test("8", "3", "2");

	printf("TEST mg_decimal_modulus(): OK\n");
}
