#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void abs_test(const char *text, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal_t value1, value2;

	mg_assert(mg_decimal_parse_string(text, &value1) == 0);

	mg_assert(mg_decimal_abs(&value1, /*out*/&value2) == 0);

	mg_assert(mg_decimal_to_string(&value2, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(ret, strbuf) == 0);
}

void decimal_abs_test()
{
	clock_t tm = clock();

	abs_test("1024", "1024");
	abs_test("-1024", "1024");
	abs_test("99999999999999999999.999999", "99999999999999999999.999999");
	abs_test("-99999999999999999999.999999", "99999999999999999999.999999");
	
	printf("TEST mg_decimal_abs(): OK\n");
}
