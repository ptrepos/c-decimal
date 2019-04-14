#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void is_zero_test(const char *text, bool zero)
{
	mg_decimal value1;

	mg_assert(mg_decimal_parse_string(text, &value1) == 0);
	mg_assert(mg_decimal_is_zero(&value1) == zero);
}

void decimal_is_zero_test()
{
	is_zero_test("1024", false);
	is_zero_test("-1024", false);
	is_zero_test("99999999999999999999.999999", false);
	is_zero_test("-99999999999999999999.999999", false);

	is_zero_test("0", true);
	is_zero_test("-0", true);
	is_zero_test("0000000000", true);
	is_zero_test("-0000000000", true);
	is_zero_test("00.00000000", true);
	is_zero_test("-00.00000000", true);

	printf("TEST mg_decimal_is_zero(): OK\n");
}
