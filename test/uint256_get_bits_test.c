#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arch_priv_uint256.h"

#include "mg_assert.h"

static void get_bits_test(const char *text1, int bits, const char *ret)
{
	char strbuf[1024];
	mg_uint256 v1;

	mg_uint256_test_hex_convert(text1, &v1);

	mg_uint256_get_bits(/*out*/&v1, bits);

	mg_uint256_test_to_hex_string(&v1, strbuf);

	mg_assert(strcmp(ret, strbuf) == 0);
}

void mg_uint256_get_bits_test()
{
	get_bits_test("FFFFFFFFFFFFFFFFFFFF", 4, "F");
	get_bits_test("FFFFFFFFFFFFFFFFFFFFFFFF", 5, "1F");
	get_bits_test("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", 8, "FF");

	get_bits_test("FFFFFFFFFFFFFFEFEFEFEFEFEFEFEFE", 32, "FEFEFEFE");
	get_bits_test("FFFFFFFFFFFFFFEFEFEFEFEFEFEFEFE", 36, "EFEFEFEFE");
	get_bits_test("FFFFFFFFFFFFFFEFEFEFEFEFEFEFEFE", 40, "FEFEFEFEFE");
	get_bits_test("FFFFFFFFFFFFFFEFEFEFEFEFEFEFEFE", 64, "FEFEFEFEFEFEFEFE");
	get_bits_test("FFFFFFFFFFEFEFEFEFEFEFEFEFEFEFE", 68, "EFEFEFEFEFEFEFEFE");
	get_bits_test("FFFFFFFFFFEFEFEFEFEFEFEFEFEFEFE", 72, "FEFEFEFEFEFEFEFEFE");

	printf("TEST mg_uint256_get_bits_test(): OK\n");
}
