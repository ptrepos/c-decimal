#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void compare_test(const char *text1, const char *text2, int ret)
{
	mg_decimal_t value1, value2;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	mg_assert(mg_decimal_parse_string(text2, &value2) == 0);

	if(ret < 0) {
		mg_assert(mg_decimal_compare(&value1, &value2) < 0);
	} else if (ret > 0) {
		mg_assert(mg_decimal_compare(&value1, &value2) > 0);
	} else {
		mg_assert(mg_decimal_compare(&value1, &value2) == 0);
	}
}

void decimal_compare_test()
{
	clock_t tm = clock();

	compare_test("0", "10000", -1);
	compare_test("10000", "0", 1);
	compare_test("-1", "0", -1);
	compare_test("0", "-1", 1);
	compare_test("0", "0", 0);
	compare_test("1000", "1000", 0);
	compare_test("100000000", "100000000", 0);
	compare_test("10000000000000000000000000", "1000", 1);
	compare_test("1000", "10000000000000000000000000", -1);
	compare_test("10000000000000000000000000", "10000000000000000000000000", 0);
	compare_test("10000000000000000000000000", "1000", 1);
	compare_test("1000", "10000000000000000000000000", -1);
	compare_test("1", "1.000", 0);
	compare_test("1", "1.001", -1);
	compare_test("2", "1.00000000000000000000000001", 1);
	compare_test("1", "1.00000000000000000000000001", -1);
	compare_test("2", "1.00000000000000000000000001", 1);
	compare_test("-1000", "-999", -1);
	compare_test("-1000.05", "-999", -1);
	compare_test("-1000", "-999.05", -1);
	compare_test("-1000", "-9900000000000009.05", 1);

	compare_test("-2", "-1.00000000000000000000000001", -1);
	compare_test("-1", "-1.00000000000000000000000001", 1);
	compare_test("-2", "-1.00000000000000000000000001", -1);

	compare_test("0.0010000", "0.0000009999999", 1);
	compare_test("-0.0010000", "-0.0000009999999", -1);

	compare_test("0.0000009999999", "0.0000009999999", 0);
	compare_test("-0.0000009999999", "-0.0000009999999", 0);

	compare_test("0.0000009999998", "0.0000009999999", -1);
	compare_test("-0.0000009999998", "-0.0000009999999", 1);

	compare_test("0.00000099999989999", "0.00000099999999", -1);
	compare_test("-0.00000099999989999", "-0.00000099999999", 1);

	compare_test("0.00000099999989999", "0.00000099899999", 1);
	compare_test("-0.00000099999989999", "-0.00000099899999", -1);

	printf("TEST mg_decimal_compare(): OK\n");
}
