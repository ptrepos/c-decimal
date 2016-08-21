#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void double_convert_test(double value, const char *ret, int precision)
{
	char strbuf[1000];
	int size;
	mg_decimal value1;
	double value2;

	mg_assert(mg_decimal_value_of_double(value, /*out*/&value1) == 0);

	mg_assert(mg_decimal_to_double(&value1, /*out*/&value2) == 0);

	mg_assert(mg_decimal_round(/*inout*/&value1, precision, MG_DECIMAL_ROUND_OFF) == 0);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(strbuf, ret) == 0);
	mg_assert(fabs(value - value2) < 0.000001);
}

static void double_convert_print(double value)
{
	char strbuf[1000];
	int size;
	mg_decimal value1;
	double value2;

	mg_assert(mg_decimal_value_of_double(value, /*out*/&value1) == 0);

	mg_assert(mg_decimal_to_double(&value1, /*out*/&value2) == 0);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	printf("org:%0.36f\ncnv:%0.36f\ndec:%s\n", value, value2, strbuf);
}

void decimal_convert_double_test()
{
	clock_t tm = clock();

	double_convert_test(0, "0", 10);
	double_convert_test(1.0, "1", 10);
	double_convert_test(1.5, "1.5", 10);
	double_convert_test(1.25, "1.25", 10);
	double_convert_test(99999999999.0, "99999999999", 10);
	double_convert_test(0.5, "0.5", 10);
	double_convert_test(0.25, "0.25", 10);
	double_convert_test(0.125, "0.125", 10);
	double_convert_test(0.625, "0.625", 10);
	double_convert_test(1.0125, "1.0125", 10);
	double_convert_test(0.0125, "0.0125", 10);
	double_convert_test(0.1, "0.1", 10);
	double_convert_test(99999999999.99, "99999999999.99", 2);
	double_convert_test(-99999999999.99, "-99999999999.99", 2);
	double_convert_test(-0.25, "-0.25", 10);
	double_convert_test(-0.125, "-0.125", 10);
	double_convert_test(-0.625, "-0.625", 10);
	double_convert_test(-965433.00163, "-965433.00163", 10);
	double_convert_test(-5555555555555555.0, "-5555555555555555", 10);
	double_convert_test(-99999999999999.0, "-99999999999999", 10);
	double_convert_test(-0.00000365242341, "-0.0000036524234", 13);
	double_convert_test(9999, "9999", 10);
	double_convert_test(456, "456", 10);
	double_convert_test(456.5, "456.5", 10);
	double_convert_test(2, "2", 10);
	double_convert_test(4, "4", 10);
	double_convert_test(8, "8", 10);
	double_convert_test(16, "16", 10);
	double_convert_test(32, "32", 10);
	double_convert_test(64, "64", 10);
	double_convert_test(128, "128", 10);
	double_convert_test(256, "256", 10);
	double_convert_test(512, "512", 10);
	double_convert_test(1024, "1024", 10);
	double_convert_test(2048, "2048", 10);
	double_convert_test(4096, "4096", 10);
	double_convert_test(8192, "8192", 10);
	double_convert_test(16384, "16384", 10);
	double_convert_test(16994, "16994", 10);
	double_convert_test(2251799813685248.0, "2251799813685248", 10);
	double_convert_test(4503599627370496.0, "4503599627370496", 10);
	double_convert_test(9007199254740992.0, "9007199254740992", 10);
	double_convert_test(18014398509481984.0, "18014398509481984", 10);
	double_convert_test(36028797018963968.0, "36028797018963968", 10);
	double_convert_test(4589599627370496.0, "4589599627370496", 10);
	double_convert_test(36695797018963968.0, "36695797018963968", 10);

	double_convert_print(-9999999999999999999.0);
	double_convert_print(9999999999999.9999);
	double_convert_print(99999999999999999999999999999999999.0);
	double_convert_print(999999999999000000000000000000000000.0);
	double_convert_print(100000000000000000000000000000000000.0);
	double_convert_print(555555555555555555550000000000000000.0);
	double_convert_print(0.00000000000000000000000000001);
	double_convert_print(0.00000000000000000000000000000000001);
	double_convert_print(0.00000000000000000000005555555);
	double_convert_print(1.0e+35);
	double_convert_print(0.00000000000000000000000000000000009);
	double_convert_print(0.00000000000000000000000000000000010);
	double_convert_print(0.00000000000000000000000000000000001);
	double_convert_print(0.000000000000000000000000000000000001);
	double_convert_print(0.00000000000000000000000000000000000009);
	double_convert_print(1.00000000000000000000000000000000009);
	double_convert_print(1.0000000000000000000000009);
	double_convert_print(1.00000000000000009);
	double_convert_print(1.000000009);
	double_convert_print(1.0009);

	{
		mg_decimal value;
		mg_assert(mg_decimal_value_of_double(1e+37, &value) == MG_DECIMAL_ERROR_OVERFLOW);
		mg_assert(mg_decimal_value_of_double(1e+39, &value) == MG_DECIMAL_ERROR_OVERFLOW);
		mg_assert(mg_decimal_value_of_double(1e+38, &value) == MG_DECIMAL_ERROR_OVERFLOW);
		mg_assert(mg_decimal_value_of_double(1e+40, &value) == MG_DECIMAL_ERROR_OVERFLOW);
		mg_assert(mg_decimal_value_of_double(1e+41, &value) == MG_DECIMAL_ERROR_OVERFLOW);
		mg_assert(mg_decimal_value_of_double(1e+42, &value) == MG_DECIMAL_ERROR_OVERFLOW);
		mg_assert(mg_decimal_value_of_double(1e+43, &value) == MG_DECIMAL_ERROR_OVERFLOW);
		mg_assert(mg_decimal_value_of_double(1e+80, &value) == MG_DECIMAL_ERROR_OVERFLOW);
		mg_assert(mg_decimal_value_of_double(1e+96, &value) == MG_DECIMAL_ERROR_OVERFLOW);
	}

	printf("TEST mg_decimal convert double methods: OK\n");
}
