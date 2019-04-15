#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/arch/uint256.h>

#include "mg_assert.h"

static void add_sub_test(const char *text1, const char *text2)
{
	char strbuf[1024];
	mg_uint256_t v1, v2;
	int borrow;

	mg_uint256_test_convert(text1, &v1);
	mg_uint256_test_convert(text2, &v2);

	mg_uint256_add(&v1, &v2);
	borrow = mg_uint256_sub(&v1, &v2);

	mg_uint256_test_to_string(&v1, strbuf);

	mg_assert(strcmp(text1, strbuf) == 0);
}

static void add128_sub128_test(const char *text1, const char *text2)
{
	char strbuf[1024];
	mg_uint256_t v1, v2;
	int borrow;

	mg_uint256_test_convert(text1, &v1);
	mg_uint256_test_convert(text2, &v2);

	mg_uint256_add128(&v1, &v2);
	borrow = mg_uint256_sub128(&v1, &v2);

	mg_uint256_test_to_string(&v1, strbuf);

	mg_assert(strcmp(text1, strbuf) == 0);
}

void mg_uint256_add_test()
{
	add_sub_test("123456789", "123465789791521321654132413241645132134411");
	add_sub_test("123465789791521321654132413241645132134411", "123465789791521321654132413241645132134411");
	add_sub_test("6324810344444111111111111111", "6324810344444111111111111111");
	add_sub_test("10", "30");
	add_sub_test("984653716745641111333333333333333333333333333333333333333333333333", "9999999");
	add_sub_test("99999999999999999999999999999999999999999999999999999999", "99999999999999999999999999999999999999999999999999999998");
	add_sub_test("99999999999999999999999999999999999999999999999999999999", "1");
	add_sub_test("99999999999999999999999999999999999999999999999999999999", "3");
	add_sub_test("99999999999999999999999999999999999999999999999999999999", "7");
	add_sub_test("3", "99999999999999999999999999999999999999999999999999999999");
	add_sub_test("7", "99999999999999999999999999999999999999999999999999999999");
	add_sub_test("88888888888881", "88888888888881");
	add_sub_test("115792089237316195423570985008687907853269984665640564039457584007913129639000", "935");
	add_sub_test("664", "1024");
	add_sub_test("1024", "1024");
	add_sub_test("97446154111", "4561");
	add_sub_test("4561", "97446154111");
	
	add128_sub128_test("664", "1024");
	add128_sub128_test("1024", "1024");
	add128_sub128_test("97446154111", "4561");
	add128_sub128_test("4561", "97446154111");

	printf("TEST mg_uint256_add() and mg_uint256_subtract(): OK\n");
}
