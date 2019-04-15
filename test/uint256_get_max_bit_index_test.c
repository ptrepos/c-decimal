#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/arch/uint256.h>

#include "mg_assert.h"

static void get_max_bit_index_test(const char *text1, int ret)
{
	mg_uint256_t v1;

	mg_uint256_test_hex_convert(text1, &v1);

	int index = mg_uint256_get_max_bit_index(&v1);

	mg_assert(ret == index);
}

void mg_uint256_get_max_bit_index_test()
{
	get_max_bit_index_test("1", 0);
	get_max_bit_index_test("FF", 7);
	get_max_bit_index_test("10FF", 12);
	get_max_bit_index_test("80FF", 15);
	get_max_bit_index_test("F0000000000", 43);
	get_max_bit_index_test("100000000", 32);
	get_max_bit_index_test("1000000000000", 48);
	get_max_bit_index_test("10000000000000000", 64);
	get_max_bit_index_test("100000000000000000000", 80);
	get_max_bit_index_test("1000000000000000000000000", 96);
	get_max_bit_index_test("1000FFFFF", 32);
	get_max_bit_index_test("1000FFFFF0000", 48);
	get_max_bit_index_test("1000FFFFF00000000", 64);
	get_max_bit_index_test("1000FFFFF000000000000", 80);
	get_max_bit_index_test("1000FFFFF0000000000000000", 96);
	get_max_bit_index_test("1000FFFFF000000000000000000000000", 128);
	get_max_bit_index_test("1000FFFFF000000000000000000000000FFFFFFFF", 160);

	printf("TEST mg_uint256_get_max_bit_index_test(): OK\n");
}
