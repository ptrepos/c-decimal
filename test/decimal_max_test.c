#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void max_test(const char *text1, const char *text2, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal value1, value2, value3;

	mg_assert(mg_decimal_parse_string(&value1, text1) == 0);
	mg_assert(mg_decimal_parse_string(&value2, text2) == 0);

	mg_assert(mg_decimal_max(&value1, &value2, /*out*/&value3) == 0);

	mg_assert(mg_decimal_to_string(&value3, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(ret, strbuf) == 0);
}

void decimal_max_test()
{
	clock_t tm = clock();

	max_test("1", "-1", "1");
	max_test("-1", "1", "1");
	max_test("-1", "-1", "-1");
	max_test("1", "1", "1");
	max_test("9999999999.9999", "999999", "9999999999.9999");
	max_test("-9999999999.9999", "999999", "999999");

	printf("TEST mg_decimal_max(): OK\n");
}
