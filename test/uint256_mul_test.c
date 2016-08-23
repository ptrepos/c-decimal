#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arch_priv_uint256.h"

#include "mg_assert.h"

static void mul_test(const char *text1, const char *text2)
{
	char strbuf[1024];
	mg_uint256 v1, v2, v3, v4;

	mg_uint256_test_convert(text1, /*out*/&v1);
	mg_uint256_test_convert(text2, /*out*/&v2);

	int overflow = mg_uint256_mul(&v1, &v2, /*out*/&v3);
	mg_assert(overflow == 0);
	overflow = mg_uint256_mul_words(&v1, MG_UINT256_WORD_COUNT, &v2, MG_UINT256_WORD_COUNT, /*out*/&v4);

	mg_uint256_test_to_string(&v1, /*out*/strbuf);

	mg_assert(strcmp(text1, /*out*/strbuf) == 0);

	mg_assert(mg_uint256_compare(&v3, &v4) == 0);

	mg_uint256_div(&v4, &v2, &v3);

	mg_assert(mg_uint256_is_zero(&v4) != 0);
	mg_assert(mg_uint256_compare(&v1, &v3) == 0);
}

static void mul128_test(const char *text1, const char *text2)
{
	mg_uint256 v1, v2, v3, v4;

	mg_uint256_test_convert(text1, &v1);
	mg_uint256_test_convert(text2, &v2);

	int overflow = mg_uint256_mul(&v1, &v2, &v3);
	mg_assert(overflow == 0);

	mg_uint256_mul128(&v1, &v2, &v4);

	mg_assert(mg_uint256_compare(&v3, &v4) == 0);
	
	mg_uint256_div(&v4, &v2, &v3);

	mg_assert(mg_uint256_is_zero(&v4) != 0);
	mg_assert(mg_uint256_compare(&v1, &v3) == 0);
}

static void mul256x64_test(const char *text1, const char *text2)
{
	mg_uint256 v1, v2, v3, v4;

	mg_uint256_test_convert(text1, &v1);
	mg_uint256_test_convert(text2, &v2);

	int overflow = mg_uint256_mul(&v1, &v2, &v3);
	mg_assert(overflow == 0);

	overflow = mg_uint256_mul256x64(&v1, &v2, &v4);

	mg_assert(overflow == 0);
	mg_assert(mg_uint256_compare(&v3, &v4) == 0);

	mg_uint256_div(&v4, &v2, &v3);

	mg_assert(mg_uint256_is_zero(&v4) != 0);
	mg_assert(mg_uint256_compare(&v1, &v3) == 0);
}

void mg_uint256_mul_test()
{
	mul_test("135123333333333333314541", "46782741153131331");
	mul_test("99999999999999999999999999999999999", "99999999999999999999999999999999999");
	mul_test("999999999999999999999", "9999999999999999");
	mul_test("500", "20");
	mul_test("8888888888888888141351", "446165153118888888888888888141351");
	mul_test("446165153118888888888888888141351", "446165153118888888888888888141351");
	mul_test("123465791234657912346579123465791234657912346579123465791234657912346579123", "20");
	mul_test("3333333333333333", "999999999999999999999999999999999999999999999999");
	mul_test("340282366920938463463374607431768211456", "1");
	mul_test("115792089237316195423570985008687907853269984665640564039457584007913129639935", "1");
	mul_test("340282366920938463463374607431768211455", "340282366920938463463374607431768211455");
	mul_test("340282366920938463463374607431768211455", "1");
	mul_test("18446744073709551616", "18446744073709551616");
	mul_test("340282366920938463463374607431768211456", "18446744073709551616");

	mul128_test("135123333333333333314541", "46782741153131331");
	mul128_test("9999999999999999999999999999999999", "9999999999999999999999999999999999");
	mul128_test("999999999999999999999", "9999999999999999");
	mul128_test("500", "20");
	mul128_test("8888888888888888141351", "446165153118888888888888888141351");
	mul128_test("446165153118888888888888888141351", "446165153118888888888888888141351");
	mul128_test("12346579123465791234657912346579123", "20");
	mul128_test("340282366920938463463374607431768211455", "340282366920938463463374607431768211455");
	mul128_test("340282366920938463463374607431768211455", "1");
	mul128_test("18446744073709551616", "18446744073709551616");

	mul256x64_test("18446744073709551616", "18446744073709551");
	mul256x64_test("12346579123465791234657912346579123", "20");
	mul256x64_test("135123333333333333314541", "5264180654151");
	mul256x64_test("135123333333333333314541", "99999999999999");
	mul256x64_test("135123333333333333314541", "2");
	mul256x64_test("135123333333333333314541", "666");
	mul256x64_test("13512333333314541", "99999999");
	mul256x64_test("1351233", "99999999");
	mul256x64_test("1351233333331454333333333331454133333333333145411", "99999999");
	mul256x64_test("3145413333333333135123333333333333314541", "99999999999999");
	mul256x64_test("3145413333333333135123333333333333314541", "9999999999999999");
	mul256x64_test("9999999999999999999999999999999999999999", "99999999999999");

	printf("TEST mg_uint256_mul(): OK\n");
}
