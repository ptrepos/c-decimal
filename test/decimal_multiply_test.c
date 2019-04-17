#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void multiply_test(const char *text1, const char *text2, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal_t value1, value2, value3;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	mg_assert(mg_decimal_parse_string(text2, &value2) == 0);

	mg_assert(mg_decimal_multiply(&value1, &value2, /*out*/&value3) == 0);

	mg_assert(mg_decimal_to_string(&value3, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(ret, strbuf) == 0);
}

static void overflow_test(const char *text1, const char *text2)
{
	mg_decimal_t value1, value2, value3;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	mg_assert(mg_decimal_parse_string(text2, &value2) == 0);

	mg_assert(mg_decimal_multiply(&value1, &value2, /*out*/&value3) == mgE_OVERFLOW);
}

void decimal_multiply_test()
{
	clock_t tm = clock();
	
	multiply_test("1000000" , "1000000", "1000000000000");
	multiply_test("1000000000000", "1000000000000", "1000000000000000000000000");
	multiply_test("-1000000", "1000000", "-1000000000000");
	multiply_test("1000000000000", "-1000000000000", "-1000000000000000000000000");
	multiply_test("1000000000000", "1000000", "1000000000000000000");
	multiply_test("1000000", "0.000001", "1");
	multiply_test("1000000000000", "0.000000000001", "1");
	multiply_test("1000000", "10.5", "10500000");
	multiply_test("1000000.555", "1000", "1000000555");
	multiply_test("1000000.000000000000555", "1000000000000000", "1000000000000000000555");
	multiply_test("1000000.000000000000555", "-1000000000000000", "-1000000000000000000555");
	multiply_test("0.00000000000000000000000000001", "100000000000000000000000000000", "1");
	multiply_test("1678165131.16", "9654.6466879411111111", "16202091425372.154318115264831876");
	multiply_test("8974165.41564", "-8974165564", "-80535646238676235.02096");
	multiply_test("11111111111165.41564", "-897411111111.65564", "-9971234567956018376497461.6691502096");
	multiply_test("11111", "100000000000000000000", "1111100000000000000000000");
	multiply_test("98765432100", "9876543210", "975461057789971041000");
	multiply_test("98.7654321009876", "987654321009.8765", "97546105780947.9682975650040314");
	multiply_test("0.1", "0.1", "0.01");
	multiply_test("0.3", "0.0333333333333333333", "0.00999999999999999999");
	multiply_test("1000", "912345756978465", "912345756978465000");
	multiply_test("999999999.9999", "999999999.9999", "999999999999800000.00000001");
	multiply_test("980.8", "3.1415926535", "3081.2740745528");
	multiply_test("1", "10", "10");
	multiply_test("1", "1000000000000", "1000000000000");
	multiply_test("89718", "8971899798271890987", "804940906101357515571666");
	multiply_test("777", "223", "173271");
	multiply_test("123456", "1.05", "129628.8");
	multiply_test("123456", "1.08", "133332.48");
	multiply_test("123456", "1.10", "135801.6");
	multiply_test("1", "0.1", "0.1");
	multiply_test("1123456", "0.000001", "1.123456");
	multiply_test("10000000000000000000000", "0.0000000000000000000001", "1");
	multiply_test("0.0001", "0.0001", "0.00000001");
	multiply_test("10000", "0.0001", "1");
	multiply_test("0.0000000001", "0.0000000001", "0.00000000000000000001");
	multiply_test("0.0000000008", "0.0000000008", "0.00000000000000000064");
	multiply_test("-0.0000000008", "0.0000000008", "-0.00000000000000000064");
	multiply_test("0.0000000008", "-0.0000000008", "-0.00000000000000000064");
	multiply_test("-0.0000000008", "-0.0000000008", "0.00000000000000000064");
	multiply_test("-10", "-10", "100");
	multiply_test("-100", "-100", "10000");
	multiply_test("-100000", "-100000", "10000000000");
	multiply_test("-100000000", "-100000000", "10000000000000000");
	multiply_test("-100000000000", "-100000000000", "10000000000000000000000");
	multiply_test("-100000000000000", "-100000000000000", "10000000000000000000000000000");
	multiply_test("1234567", "1234567", "1524155677489");
	multiply_test("123456789", "123456789", "15241578750190521");
	multiply_test("1234567891234", "1234567891234", "1524157878065965654042756");
	multiply_test("123456789123456", "123456789123456", "15241578780673483700809383936");
	multiply_test("12345678912345600", "12345678912345600", "152415787806734837008093839360000");
	multiply_test("239541530454123456978132", "3333", "798391921003593482108113956");
	multiply_test("12331", "239541530454123456978132", "2953786612029796347997345692");
	multiply_test("20498", "0.78", "15988.44");
	multiply_test("9979", "0.87", "8681.73");
	
	multiply_test("0.00000000123412", "89145604156", "110.01637300100272");
	multiply_test("89145604156", "0.00000000123412", "110.01637300100272");
	multiply_test("0.00000000123412", "0.0000012312", "0.000000000000001519448544");
	multiply_test("0.0000012312", "0.00000000123412", "0.000000000000001519448544");
	multiply_test("-0.00000000123412", "0.0000012312", "-0.000000000000001519448544");
	multiply_test("0.0000012312", "-0.00000000123412", "-0.000000000000001519448544");
	multiply_test("-0.00000000123412", "89145604156", "-110.01637300100272");
	multiply_test("89145604156", "-0.00000000123412", "-110.01637300100272");
	multiply_test("0.00000000123412", "-0.0000012312", "-0.000000000000001519448544");
	multiply_test("-0.0000012312", "0.00000000123412", "-0.000000000000001519448544");
	multiply_test("0.00000000123412", "-89145604156", "-110.01637300100272");
	multiply_test("-89145604156", "0.00000000123412", "-110.01637300100272");

	overflow_test("99999999999999999999999999.0", "99999999999999999999999999.0");
	overflow_test("199999999999999999999999999.0", "199999999999999999999999999.0");

	printf("TEST mg_decimal_multiply(): OK\n");
}
