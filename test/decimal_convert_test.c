#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void convert_test(const char *text)
{
	char strbuf[1000];
	int size;
	mg_decimal value;
	
	mg_assert(mg_decimal_parse_string(text, &value) == 0);
	mg_assert(mg_decimal_to_string(&value, strbuf, 0, &size) != 0);
	mg_assert(mg_decimal_to_string(&value, strbuf, size, &size) == 0);

	mg_assert(strcmp(text, strbuf) == 0);
}

static void convert_test2(const char *text1, const char *text2)
{
	char strbuf[1000];
	int size;
	mg_decimal value;

	mg_assert(mg_decimal_parse_string(text1, &value) == 0);
	mg_assert(mg_decimal_to_string(&value, strbuf, 0, &size) != 0);
	mg_assert(mg_decimal_to_string(&value, strbuf, size, &size) == 0);

	mg_assert(strcmp(text2, strbuf) == 0);
}

static void int64_convert_test(int64_t value, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal value1;
	int64_t value2;

	mg_assert(mg_decimal_value_of_int64(value, &value1) == 0);

	mg_assert(mg_decimal_to_int64(&value1, /*out*/&value2) == 0);

	mg_assert(value == value2);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(strbuf, ret) == 0);
}

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

void decimal_convert_test()
{
	clock_t tm = clock();

	convert_test("-1");
	convert_test("0");
	convert_test("1");
	convert_test("123456789");
	convert_test("123456789123456789");
	convert_test("12345678912345678912345678912");
	convert_test("1234567.1001");
	convert_test("1234567.1234567894656");
	convert_test("1234561234567.1234567");
	convert_test("1234561234567894567.1234567");
	convert_test("12345612345678123194567.123456");
	convert_test("12345612345678123194567.123456");
	convert_test("12345612345678123194567.123456");
	convert_test("98884168416516413165164841654");
	convert_test("9888416841651641316.5164841654");
	convert_test("988841684.16516413165164841654");
	convert_test("78978.111111111111111111111111");
	convert_test("7.1111111111111111111111111111");
	convert_test("0.11111111111111111111111111111");
	convert_test("0.11111111897684154314546454441");
	convert_test("0.13111111897684154314546454441");
	convert_test("-6666");
	convert_test("-6666666666666666666666666");
	convert_test("-6666.6666");
	convert_test("-6666666666666666.666666666");
	convert_test("808465479.056146514");
	convert_test("8089.46547056146514");
	convert_test("808946547056146.514");
	convert_test("8089465470561489451411111114");
	convert_test("1000000.000000000000555");
	convert_test("1000000000000000");
	convert_test("100000000000000000000");
	convert_test("999999999999999999999999999999999999");
	
	convert_test2("00000111111111111", "111111111111");
	convert_test2("-00000111111111111", "-111111111111");
	convert_test2("+00000111111111111", "111111111111");
	convert_test2("+000001111111.11111", "1111111.11111");
	convert_test2("1.10", "1.1");
	convert_test2("1.10000", "1.1");
	convert_test2("1.100000000000", "1.1");
	convert_test2("1.1000000000000000000000000", "1.1");
	convert_test2("1.01234564857894156327864986148541864564165411231321", "1.01234564857894156327864986148541864");
	convert_test2("0.11234564857894156327864986148541864564165411231321", "0.112345648578941563278649861485418645");
	
	int64_convert_test(1000, "1000");
	int64_convert_test(-1000, "-1000");
	int64_convert_test(-9999999999999, "-9999999999999");
	int64_convert_test(-999999999999999999, "-999999999999999999");
	int64_convert_test(-425415311, "-425415311");

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


	{
		mg_decimal value;
		mg_assert(mg_decimal_parse_string("1234567891234156498715634865156465151654152165453132416854114", &value) == MG_DECIMAL_ERROR_OVERFLOW);
	}

	printf("TEST mg_decimal convert methods: OK\n");
}
