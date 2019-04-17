#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void negative_test(const char *text, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal_t value;

	mg_assert(mg_decimal_parse_string(text, &value) == 0);

	mg_assert(mg_decimal_negate(/*inout*/&value) == 0);

	mg_assert(mg_decimal_to_string(/*out*/&value, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(ret, strbuf) == 0);
}

void decimal_negative_test()
{
	clock_t tm = clock();

	negative_test("1", "-1");
	negative_test("0", "0");
	negative_test("-1", "1");
	negative_test("-100000000000000000000", "100000000000000000000");
	negative_test("100000000000000000000", "-100000000000000000000");
	negative_test("-312654515641615641", "312654515641615641");
	negative_test("312654515641615641", "-312654515641615641");
	negative_test("-3126545156416156412134111101544963", "3126545156416156412134111101544963");
	negative_test("3126545156416156412134111101544963", "-3126545156416156412134111101544963");
	negative_test("-18446744073709551616", "18446744073709551616");
	negative_test("18446744073709551616", "-18446744073709551616");
	negative_test("-31265", "31265");
	negative_test("31265", "-31265");

	printf("TEST mg_decimal_negate(): OK\n");
}
