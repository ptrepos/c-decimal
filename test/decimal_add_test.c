#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void add_test(const char *text1, const char *text2, const char *ret)
{
	char strbuf[1000];
	int size;
	mg_decimal_t value1, value2, value3;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	mg_assert(mg_decimal_parse_string(text2, &value2) == 0);

	mg_assert(mg_decimal_add(&value1, &value2, /*out*/&value3) == 0);

	mg_assert(mg_decimal_to_string(&value3, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(ret, strbuf) == 0);
}

void decimal_add_test()
{
	clock_t tm = clock();

	add_test("1", "1", "2");
	add_test("1", "-1", "0");
	add_test("1", "0", "1");
	add_test("0", "1", "1");
	add_test("100000000000000000000", "10000000000000000", "100010000000000000000");
	add_test("100000000000000000000", "-10000000000000000", "99990000000000000000");
	add_test("-100000000000000000000", "10000000000000000", "-99990000000000000000");
	add_test("-100000000000000000000", "-10000000000000000", "-100010000000000000000");
	add_test("-1000000.000001", "-99.99", "-1000099.990001");
	add_test("-1000000.000001", "-99.000001", "-1000099.000002");
	add_test("1000000000.00000000000000001", "999999999.9999999999999999", "1999999999.99999999999999991");
	add_test("1000000000.00000000000000001", "-999999999.9999999999999999", "0.00000000000000011");
	add_test("-1000000000.00000000000000001", "999999999.9999999999999999", "-0.00000000000000011");
	add_test("-1000000000.00000000000000001", "-999999999.9999999999999999", "-1999999999.99999999999999991");
	add_test("9999999999999999999999999999999998", "1", "9999999999999999999999999999999999");
	add_test("3295", "8", "3303");
	add_test("9996666", "3334", "10000000");
	add_test("99999999999999999999", "1", "100000000000000000000");
	// The decimal is round down on overflow
	add_test("123456789123456789123456789", "0.12345678912345678912345678901234", "123456789123456789123456789.123456789");
	add_test("1678165131.16", "9654.6466879411111111", "1678174785.8066879411111111");
	add_test("8974165.41564", "-8974165564", "-8965191398.58436");
	add_test("11111111111165.41564", "-897411111111.65564", "10213700000053.76");
	add_test("11111", "100000000000000000000", "100000000000000011111");
	add_test("987654321009876543210", "987654321009876543210", "1975308642019753086420");
	add_test("98.7654321009876543210", "987654321009.876543210", "987654321108.641975310987654321");
	add_test("0.1", "0.1", "0.2");
	add_test("0.3", "0.0333333333333333333", "0.3333333333333333333");
	add_test("1000", "912345756978465", "912345756979465");
	add_test("999999999.9999", "999999999.9999", "1999999999.9998");
	add_test("980.8", "3.1415926535", "983.9415926535");
	add_test("1", "10", "11");
	add_test("1", "1000000000000", "1000000000001");
	add_test("89718", "8971899798271890987", "8971899798271980705");
	add_test("777", "223", "1000");
	add_test("123456", "1.05", "123457.05");
	add_test("123456", "1.08", "123457.08");
	add_test("123456", "1.10", "123457.1");
	add_test("-10000000000000000000000000000", "-0.000000000000000000000000000000001", "-10000000000000000000000000000");
	add_test("100", "-110", "-10");
	add_test("100", "-999", "-899");
	add_test("10000000000000000000000", "-11000000000000000000000", "-1000000000000000000000");
	add_test("1000000.00", "1000000.00", "2000000");
	add_test("1000000.00", "-1000000.00", "0");
	add_test("-1000000.00", "1000000.00", "0");
	add_test("1000000.99", "-1000000.00", "0.99");
	add_test("-1000000.00", "1000000.99", "0.99");
	add_test("-1000000.00", "-1000000.99", "-2000000.99");
	add_test("100000000000000000000000000.00", "100000000000000000000000000.00", "200000000000000000000000000");
	add_test("100000000000000000000000000.00", "-100000000000000000000000000.00", "0");
	add_test("-100000000000000000000000000.00", "100000000000000000000000000.00", "0");
	add_test("100000000000000000000000000.99", "-100000000000000000000000000.00", "0.99");
	add_test("-100000000000000000000000000.00", "100000000000000000000000000.99", "0.99");
	add_test("-100000000000000000000000000.00", "-100000000000000000000000000.99", "-200000000000000000000000000.99");
	add_test("100000000000.55", "10000000000000.000000000999", "10100000000000.550000000999");
	add_test("100000000000.55", "-10000000000000.000000000999", "-9899999999999.450000000999");
	add_test("-100000000000.55", "10000000000000.000000000999", "9899999999999.450000000999");
	add_test("10000000000000.000000000999", "-100000000000.55", "9899999999999.450000000999");
	add_test("-10000000000000.000000000999", "100000000000.55", "-9899999999999.450000000999");
	add_test("-10000000000000.000000000999", "-100000000000.55", "-10100000000000.550000000999");
	add_test("0", "10000000000000.000000000999", "10000000000000.000000000999");
	add_test("10000000000000.000000000999", "0", "10000000000000.000000000999");
	add_test("0", "-10000000000000.000000000999", "-10000000000000.000000000999");
	add_test("-10000000000000.000000000999", "0", "-10000000000000.000000000999");
	add_test("1000.0005", "1000.0005", "2000.001");
	add_test("1000.0005", "100.0005", "1100.001");
	add_test("1000.0005", "10.0005", "1010.001");
	add_test("1000.0005", "1.0005", "1001.001");
	add_test("999999999999999999999999999999.0", "0.000000000000000000001", "999999999999999999999999999999");
	add_test("999999999999999999999999999999.0", "0.0000000000000000009999999999", "999999999999999999999999999999");
	add_test("999999999999999999999999999999.0", "-0.0000000000000000009999999999", "999999999999999999999999999998.999999");

	printf("TEST mg_decimal_add(): OK\n");
}
