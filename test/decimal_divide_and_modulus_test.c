#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void divide_and_modulus_test(const char *text1, const char *text2, const char *ret, const char *reminder)
{
	char strbuf[1000];
	char strbuf2[1000];
	int size;
	mg_decimal_t value1, value2, value3, value4;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	mg_assert(mg_decimal_parse_string(text2, &value2) == 0);

	mg_assert(mg_decimal_divide_and_modulus(&value1, &value2, /*out*/&value3, /*out*/&value4) == 0);

	mg_assert(mg_decimal_to_string(&value3, strbuf, 1000, &size) == 0);
	mg_assert(mg_decimal_to_string(&value4, strbuf2, 1000, &size) == 0);

	mg_assert(strcmp(ret, strbuf) == 0);
	mg_assert(strcmp(reminder, strbuf2) == 0);
}

static void divide_and_modulus_test2(const char *text1, const char *text2)
{
	mg_decimal_t value1, value2;
	mg_decimal_t q, r, tmp;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	mg_assert(mg_decimal_parse_string(text2, &value2) == 0);

	mg_assert(mg_decimal_divide_and_modulus(&value1, &value2, &q, &r) == 0);

	mg_assert(mg_decimal_multiply(&q, &value2, /*out*/&tmp) == 0);
	mg_assert(mg_decimal_add(&tmp, &r, /*out*/&tmp) == 0);

	char strbuf[1000];
	char strbuf2[1000];
	char strbuf3[1000];
	int size;
	mg_assert(mg_decimal_to_string(&q, strbuf, 1000, &size) == 0);
	mg_assert(mg_decimal_to_string(&r, strbuf2, 1000, &size) == 0);
	mg_assert(mg_decimal_to_string(&tmp, strbuf3, 1000, &size) == 0);

	mg_assert(mg_decimal_compare(&tmp, &value1) == 0);
}

void decimal_divide_and_modulus_test()
{
	clock_t tm = clock();

	divide_and_modulus_test("9999999999999999999999999999999999", "9999999999999999999999999999999999", "1", "0");
	divide_and_modulus_test("-111111111111111111111", "-1000", "111111111111111111", "-111");
	divide_and_modulus_test("-111111111111111111111", "-1000", "111111111111111111", "-111");
	divide_and_modulus_test("1000000000000000000000000000", "3", "333333333333333333333333333", "1");
	divide_and_modulus_test("1959", "2", "979", "1");
	divide_and_modulus_test("100", "0.01", "10000", "0");
	divide_and_modulus_test("100.01", "0.01", "10001", "0");
	divide_and_modulus_test("100.005000000", "0.06", "1666", "0.045");
	divide_and_modulus_test("100.005", "0.06", "1666", "0.045");
	divide_and_modulus_test("100.11111", "0.1", "1001", "0.01111");
	divide_and_modulus_test("19999.999", "32", "624", "31.999");
	divide_and_modulus_test("1", "0.000000001", "1000000000", "0");
	divide_and_modulus_test("984615112", "15641", "62950", "14162");
	divide_and_modulus_test("3212313211512654132123441131515", "3212313211512654132123441131515", "1", "0");
	divide_and_modulus_test("3212313211512654132123441131515", "321231321151265413212344", "10000000", "1131515");
	divide_and_modulus_test("3212313211512654132123441131515", "3212313211512654132", "1000000000000", "123441131515");
	divide_and_modulus_test("3212313211512654132123441131515", "321231321151", "10000000000008262370", "92502743645");
	divide_and_modulus_test("3212313211512654132123441131515", "321231", "10000009997517842711704166", "183169");
	divide_and_modulus_test("3212313211512654132123441131515", "2", "1606156605756327066061720565757", "1");
	divide_and_modulus_test("3212313211512654132123441131515", "1", "3212313211512654132123441131515", "0");
	divide_and_modulus_test("32123132115126541321234.41131515", "3212313211512654132123441131515", "0", "32123132115126541321234.41131515");
	divide_and_modulus_test("32123132115126541321234.41131515", "321231321151265413212344", "0", "32123132115126541321234.41131515");
	divide_and_modulus_test("321231321151265413212344.1131515", "3212313211512654132", "100000", "12344.1131515");
	divide_and_modulus_test("321231321151265413212.3441131515", "321231321151", "1000000000", "265413212.3441131515");
	divide_and_modulus_test("321231321151265413.2123441131515", "321231", "1000000999751", "251932.2123441131515");
	divide_and_modulus_test("3212313211512654132123441.131515", "2", "1606156605756327066061720", "1.131515");
	divide_and_modulus_test("321231321151265413.2123441131515", "1", "321231321151265413", "0.2123441131515");
	divide_and_modulus_test("18446744073709551616", "18446744073709551616", "1", "0");
	divide_and_modulus_test("18446744073709551616", "500000000", "36893488147", "209551616");
	divide_and_modulus_test("18446744073709551616", "35.300", "522570653646162935", "10.5");
	divide_and_modulus_test("18446744073709551616", "3500000.300000", "5270497855017", "695110.9");
	divide_and_modulus_test("-18446744073709551616", "18446744073709551616", "-1", "0");
	divide_and_modulus_test("18446744073709551616", "-500000000", "-36893488147", "209551616");
	divide_and_modulus_test("-18446744073709551616", "-35.300", "522570653646162935", "-10.5");

	// divide/modulusåüéZ
	divide_and_modulus_test2("546456040", "4654156");
	divide_and_modulus_test2("1", "3");
	divide_and_modulus_test2("1", "-3");
	divide_and_modulus_test2("-1", "-3");
	divide_and_modulus_test2("999999956465465.141", "33.3");
	divide_and_modulus_test2("987456", "62.5");
	divide_and_modulus_test2("333333333333", "3");
	divide_and_modulus_test2("987654321", "11111111111111111111");
	divide_and_modulus_test2("11111111111111111111", "9871987");
	divide_and_modulus_test2("1111111111111.1111111", "987.1987");
	divide_and_modulus_test2("-99999995.141", "-33.64654653");
	divide_and_modulus_test2("33.33333", "33.3");
	divide_and_modulus_test2("33300000000000000000000", "33.3");
	divide_and_modulus_test2("33300000000000000000001", "33.3");
	divide_and_modulus_test2("391887191456", "35215331.156234");
	divide_and_modulus_test2("9875424163165", "3");
	divide_and_modulus_test2("21121", "22332323");
	divide_and_modulus_test2("15000.0004", "15000.0004");
	divide_and_modulus_test2("15000.0004", "15000.0004");
	divide_and_modulus_test2("-15000.0004", "15000.0004");
	divide_and_modulus_test2("798798798111789784561211144", "7987987987987987.98798");
	divide_and_modulus_test2("798798798111789784561211144", "-7987987987987987.98798");
	//divide_and_modulus_test2("798798798111789784561211112944", "79.8798798798798798798"); //@J åvéZåÎç∑Ç≈ïsàÍív
	divide_and_modulus_test2("79879879811178978456121111294489", "1000000000000000000000");
	divide_and_modulus_test2("798798798111789784561211112944891", "1000000000000000000000");
	divide_and_modulus_test2("150.4", "1500");
	divide_and_modulus_test2("8888888888888888", "2");
	divide_and_modulus_test2("8888888888888888", "4");
	divide_and_modulus_test2("8888888888888888", "8");
	divide_and_modulus_test2("8888888888888888", "3");
	divide_and_modulus_test2("8888888888888888", "-2");
	divide_and_modulus_test2("8888888888888888", "-4");
	divide_and_modulus_test2("8888888888888888", "-8");
	divide_and_modulus_test2("8888888888888888", "-3");
	divide_and_modulus_test2("-8888888888888888", "-2");
	divide_and_modulus_test2("-8888888888888888", "-4");
	divide_and_modulus_test2("-8888888888888888", "-8");
	divide_and_modulus_test2("-8888888888888888", "-3");
	divide_and_modulus_test2("-8888888888888888", "2");
	divide_and_modulus_test2("-8888888888888888", "4");
	divide_and_modulus_test2("-8888888888888888", "8");
	divide_and_modulus_test2("-8888888888888888", "3");
	divide_and_modulus_test2("123456", "0.00000000000000000000000001");
	divide_and_modulus_test2("-48941513131", "0.000000000461651");
	divide_and_modulus_test2("-4894151", "0.000000461651");
	divide_and_modulus_test2("-897798788465145641454134", "0.0000000003");
	divide_and_modulus_test2("-897798", "0.000000000000000084615533");
	divide_and_modulus_test2("897798", "-0.000000000000000084615533");
	divide_and_modulus_test2("-897798", "-0.000000000000000084615533");
	divide_and_modulus_test2("-897798999999999999", "-0.003");
	divide_and_modulus_test2("-89779899999999999131539", "-0.099");
	divide_and_modulus_test2("-0.0000000000008", "254");
	divide_and_modulus_test2("-0.0000000000008", "0.00000000000000254");
	divide_and_modulus_test2("-0.0000000000008", "0.0000000000000254");
	divide_and_modulus_test2("-0.0000000000008", "0.000000000000254");
	divide_and_modulus_test2("-0.0000000000008", "0.00000000000254");
	divide_and_modulus_test2("-0.0000000000008", "0.0000000000254");
	divide_and_modulus_test2("-897798999999999991315", "-0.099");
	divide_and_modulus_test2("-8977989999999999913", "-0.099");
	divide_and_modulus_test2("-8977989999999999", "-0.099");
	divide_and_modulus_test2("-89779899999999", "-0.099");
	divide_and_modulus_test2("-897798999999", "-0.099");
	divide_and_modulus_test2("-8977989999", "-0.099");
	divide_and_modulus_test2("-89779899", "-0.099");
	divide_and_modulus_test2("-897798", "-0.099");
	divide_and_modulus_test2("-8977", "-0.099");
	divide_and_modulus_test2("-89", "-0.099");

	printf("TEST mg_decimal_divide_and_modulus(): OK\n");
}
