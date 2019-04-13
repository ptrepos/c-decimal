#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void round_test(const char *text, int precision, int type, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal value1;

	mg_assert(mg_decimal_parse_string(text, &value1) == 0);

	mg_assert(mg_decimal_round(/*inout*/&value1, precision, type) == 0);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(ret, strbuf) == 0);
}

void decimal_round_test()
{
	clock_t tm = clock();

	round_test("10.0015", 3, MG_DECIMAL_ROUND_UP, "10.002");
	round_test("10.0015", 3, MG_DECIMAL_ROUND_DOWN, "10.001");
	round_test("10.0015", 3, MG_DECIMAL_ROUND_OFF, "10.002");
	round_test("10.0014", 3, MG_DECIMAL_ROUND_OFF, "10.001");
	round_test("10.0014", 3, MG_DECIMAL_ROUND_UP, "10.002");
	round_test("10.0014", 3, MG_DECIMAL_ROUND_DOWN, "10.001");
	round_test("10.000000000000000000000015", 23, MG_DECIMAL_ROUND_UP, "10.00000000000000000000002");
	round_test("10.000000000000000000000015", 23, MG_DECIMAL_ROUND_DOWN, "10.00000000000000000000001");
	round_test("10.000000000000000000000015", 23, MG_DECIMAL_ROUND_OFF, "10.00000000000000000000002");
	round_test("10.000000000000000000000014", 23, MG_DECIMAL_ROUND_OFF, "10.00000000000000000000001");
	round_test("10.000000000000000000000014", 23, MG_DECIMAL_ROUND_UP, "10.00000000000000000000002");
	round_test("10.000000000000000000000014", 23, MG_DECIMAL_ROUND_DOWN, "10.00000000000000000000001");

	round_test("10.001412345679", 3, MG_DECIMAL_ROUND_DOWN, "10.001");
	round_test("10.001512345679", 3, MG_DECIMAL_ROUND_DOWN, "10.001");
	round_test("10.00141", 3, MG_DECIMAL_ROUND_DOWN, "10.001");
	round_test("10.00159", 3, MG_DECIMAL_ROUND_DOWN, "10.001");
	round_test("10.00141211", 3, MG_DECIMAL_ROUND_DOWN, "10.001");
	round_test("10.00151111", 3, MG_DECIMAL_ROUND_DOWN, "10.001");

	round_test("10.001412345679", 3, MG_DECIMAL_ROUND_UP, "10.002");
	round_test("10.001512345679", 3, MG_DECIMAL_ROUND_UP, "10.002");
	round_test("10.00141", 3, MG_DECIMAL_ROUND_UP, "10.002");
	round_test("10.00159", 3, MG_DECIMAL_ROUND_UP, "10.002");
	round_test("10.00141211", 3, MG_DECIMAL_ROUND_UP, "10.002");
	round_test("10.00151111", 3, MG_DECIMAL_ROUND_UP, "10.002");

	round_test("10.001412345679", 3, MG_DECIMAL_ROUND_OFF, "10.001");
	round_test("10.001512345679", 3, MG_DECIMAL_ROUND_OFF, "10.002");
	round_test("10.00141", 3, MG_DECIMAL_ROUND_OFF, "10.001");
	round_test("10.00159", 3, MG_DECIMAL_ROUND_OFF, "10.002");
	round_test("10.00141211", 3, MG_DECIMAL_ROUND_OFF, "10.001");
	round_test("10.00151111", 3, MG_DECIMAL_ROUND_OFF, "10.002");

	round_test("-10.001412345679", 3, MG_DECIMAL_ROUND_DOWN, "-10.001");
	round_test("-10.001512345679", 3, MG_DECIMAL_ROUND_DOWN, "-10.001");
	round_test("-10.00141", 3, MG_DECIMAL_ROUND_DOWN, "-10.001");
	round_test("-10.00159", 3, MG_DECIMAL_ROUND_DOWN, "-10.001");
	round_test("-10.00141211", 3, MG_DECIMAL_ROUND_DOWN, "-10.001");
	round_test("-10.00151111", 3, MG_DECIMAL_ROUND_DOWN, "-10.001");

	round_test("-10.001412345679", 3, MG_DECIMAL_ROUND_UP, "-10.002");
	round_test("-10.001512345679", 3, MG_DECIMAL_ROUND_UP, "-10.002");
	round_test("-10.00141", 3, MG_DECIMAL_ROUND_UP, "-10.002");
	round_test("-10.00159", 3, MG_DECIMAL_ROUND_UP, "-10.002");
	round_test("-10.00141211", 3, MG_DECIMAL_ROUND_UP, "-10.002");
	round_test("-10.00151111", 3, MG_DECIMAL_ROUND_UP, "-10.002");

	round_test("-10.001412345679", 3, MG_DECIMAL_ROUND_OFF, "-10.001");
	round_test("-10.001512345679", 3, MG_DECIMAL_ROUND_OFF, "-10.002");
	round_test("-10.00141", 3, MG_DECIMAL_ROUND_OFF, "-10.001");
	round_test("-10.00159", 3, MG_DECIMAL_ROUND_OFF, "-10.002");
	round_test("-10.00141211", 3, MG_DECIMAL_ROUND_OFF, "-10.001");
	round_test("-10.00151111", 3, MG_DECIMAL_ROUND_OFF, "-10.002");

	round_test("10.001412345679", 3, MG_DECIMAL_CEILING, "10.002");
	round_test("10.001512345679", 3, MG_DECIMAL_CEILING, "10.002");
	round_test("-10.001412345679", 3, MG_DECIMAL_CEILING, "-10.001");
	round_test("-10.001512345679", 3, MG_DECIMAL_CEILING, "-10.001");

	round_test("0000.001412345679", 3, MG_DECIMAL_CEILING, "0.002");
	round_test("0000.001512345679", 3, MG_DECIMAL_CEILING, "0.002");
	round_test("-0000.001412345679", 3, MG_DECIMAL_CEILING, "-0.001");
	round_test("-0000.001512345679", 3, MG_DECIMAL_CEILING, "-0.001");

	round_test("10.001412345679", 3, MG_DECIMAL_FLOOR, "10.001");
	round_test("10.001512345679", 3, MG_DECIMAL_FLOOR, "10.001");
	round_test("-10.001412345679", 3, MG_DECIMAL_FLOOR, "-10.002");
	round_test("-10.001512345679", 3, MG_DECIMAL_FLOOR, "-10.002");

	round_test("0000.001412345679", 3, MG_DECIMAL_FLOOR, "0.001");
	round_test("0000.001512345679", 3, MG_DECIMAL_FLOOR, "0.001");
	round_test("-0000.001412345679", 3, MG_DECIMAL_FLOOR, "-0.002");
	round_test("-0000.001512345679", 3, MG_DECIMAL_FLOOR, "-0.002");

	printf("TEST mg_decimal_round(): OK\n");
}
