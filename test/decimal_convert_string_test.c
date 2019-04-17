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
	mg_decimal_t value;
	
	mg_assert(mg_decimal_parse_string(text, &value) == 0);
	mg_assert(mg_decimal_to_string(&value, strbuf, 0, &size) != 0);
	mg_assert(mg_decimal_to_string(&value, strbuf, size, &size) == 0);

	mg_assert(strcmp(text, strbuf) == 0);
}

static void convert_test2(const char *text1, const char *text2)
{
	char strbuf[1000];
	int size;
	mg_decimal_t value;

	mg_assert(mg_decimal_parse_string(text1, &value) == 0);
	mg_assert(mg_decimal_to_string(&value, strbuf, 0, &size) != 0);
	mg_assert(mg_decimal_to_string(&value, strbuf, size, &size) == 0);

	mg_assert(strcmp(text2, strbuf) == 0);
}

void decimal_convert_string_test()
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
	
	{
		mg_decimal_t value;
		mg_assert(mg_decimal_parse_string("1234567891234156498715634865156465151654152165453132416854114", &value) == mgE_OVERFLOW);
	}

	printf("TEST mg_decimal convert string methods: OK\n");
}
