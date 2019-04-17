#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void float_convert_test(float value, const char *ret, int precision)
{
	char strbuf[1000];
	int size;
	mg_decimal_t value1;
	float value2;

	mg_assert(mg_decimal_value_of_float(value, /*out*/&value1) == 0);

	mg_assert(mg_decimal_to_float(&value1, /*out*/&value2) == 0);

	mg_assert(mg_decimal_round(/*inout*/&value1, precision, mgDECIMAL_ROUND_OFF) == 0);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(strbuf, ret) == 0);
	mg_assert(fabs(value - value2) < 0.000001);
}

void decimal_convert_float_test()
{
	clock_t tm = clock();

	float_convert_test(0.0f, "0", 10);
	float_convert_test(1.0f, "1", 10);
	float_convert_test(1.5f, "1.5", 10);
	float_convert_test(1.25f, "1.25", 10);
	float_convert_test(9999999.0f, "9999999", 10);
	float_convert_test(0.5f, "0.5", 6);
	float_convert_test(0.25f, "0.25", 6);
	float_convert_test(0.125f, "0.125", 6);
	float_convert_test(0.625f, "0.625", 6);
	float_convert_test(1.0125f, "1.0125", 6);
	float_convert_test(0.0125f, "0.0125", 6);
	float_convert_test(0.1f, "0.1", 6);
	float_convert_test(9999.99f, "9999.99", 3);
	float_convert_test(-9999.99f, "-9999.99", 3);
	float_convert_test(-0.25, "-0.25", 6);
	float_convert_test(-0.125, "-0.125", 6);
	float_convert_test(-0.625, "-0.625", 6);
	float_convert_test(9999, "9999", 6);
	float_convert_test(456, "456", 6);
	float_convert_test(456.5, "456.5", 6);
	float_convert_test(2, "2", 6);
	float_convert_test(4, "4", 6);
	float_convert_test(8, "8", 6);
	float_convert_test(16, "16", 6);
	float_convert_test(32, "32", 6);
	float_convert_test(64, "64", 6);
	float_convert_test(128, "128", 6);
	float_convert_test(256, "256", 6);
	float_convert_test(512, "512", 6);
	float_convert_test(1024, "1024", 6);
	float_convert_test(2048, "2048", 6);
	float_convert_test(4096, "4096", 6);
	float_convert_test(8192, "8192", 6);
	float_convert_test(16384, "16384", 6);
	float_convert_test(16994, "16994", 6);

	{
		mg_decimal_t value;
		mg_assert(mg_decimal_value_of_float(1e+37f, &value) == mgE_OVERFLOW);
	}

	printf("TEST mg_decimal convert float methods: OK\n");
}
