#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arch_priv_uint256.h"

#include "mg_assert.h"

static void left_shift_test(const char *text1, int shift, const char *ret)
{
	char strbuf[1024];
	mg_uint256_t v1;

	mg_uint256_test_hex_convert(text1, &v1);

	mg_uint256_left_shift(&v1, shift);

	mg_uint256_test_to_hex_string(&v1, strbuf);

	mg_assert(strcmp(ret, strbuf) == 0);
}

void mg_uint256_left_shift_test()
{
	left_shift_test("FF", 1, "1FE");
	left_shift_test("FF0000000000000000000", 1, "1FE0000000000000000000");
	left_shift_test("FF000000000000000000000000000000", 1, "1FE000000000000000000000000000000");
	left_shift_test("FF00000000000000000000000000000000000000", 1, "1FE00000000000000000000000000000000000000");
	left_shift_test("FF0000000000000000000000000000000000000000000000", 1, "1FE0000000000000000000000000000000000000000000000");

	left_shift_test("FF", 16, "FF0000");
	left_shift_test("FF0000000000000000000", 16, "FF00000000000000000000000");
	left_shift_test("FF000000000000000000000000000000", 16, "FF0000000000000000000000000000000000");
	left_shift_test("FF00000000000000000000000000000000000000", 16, "FF000000000000000000000000000000000000000000");
	left_shift_test("FF0000000000000000000000000000000000000000000000", 16, "FF00000000000000000000000000000000000000000000000000");

	left_shift_test("FF", 20, "FF00000");
	left_shift_test("FF0000000000000000000", 20, "FF000000000000000000000000");
	left_shift_test("FF000000000000000000000000000000", 20, "FF00000000000000000000000000000000000");
	left_shift_test("FF00000000000000000000000000000000000000", 20, "FF0000000000000000000000000000000000000000000");
	left_shift_test("FF0000000000000000000000000000000000000000000000", 20, "FF000000000000000000000000000000000000000000000000000");

	left_shift_test("FF", 33, "1FE00000000");
	left_shift_test("FF00000000", 33, "1FE0000000000000000");
	left_shift_test("FF0000000000000000", 33, "1FE000000000000000000000000");
	left_shift_test("FF000000000000000000000000", 33, "1FE00000000000000000000000000000000");

	left_shift_test("FF", 66, "3FC0000000000000000");
	left_shift_test("FF00000000", 66, "3FC000000000000000000000000");
	left_shift_test("FF0000000000000000", 66, "3FC00000000000000000000000000000000");
	left_shift_test("FF0000000000000000000000000", 66, "3FC00000000000000000000000000000000000000000");

	left_shift_test("FF", 128, "FF00000000000000000000000000000000");
	left_shift_test("FF000000000000000000000000000000", 128, "FF00000000000000000000000000000000000000000000000000000000000000");
	left_shift_test("FF00000000000000000000000000000000", 128, "0");

	left_shift_test("FF", 160, "FF0000000000000000000000000000000000000000");
	left_shift_test("FF0000", 160, "FF00000000000000000000000000000000000000000000");
	left_shift_test("FF00000000", 160, "FF000000000000000000000000000000000000000000000000");

	left_shift_test("FF", 164, "FF00000000000000000000000000000000000000000");
	left_shift_test("FF0000", 164, "FF000000000000000000000000000000000000000000000");
	left_shift_test("FF00000000", 164, "FF0000000000000000000000000000000000000000000000000");

	left_shift_test("FF0FF00FF00FF00FF00FF", 40, "FF0FF00FF00FF00FF00FF0000000000");
	left_shift_test("FF0FF00FF00FF00FF00FF", 60, "FF0FF00FF00FF00FF00FF000000000000000");
	left_shift_test("FF0FF00FF00FF00FF00FF", 80, "FF0FF00FF00FF00FF00FF00000000000000000000");
	left_shift_test("FF0FF00FF00FF00FF00FF", 100, "FF0FF00FF00FF00FF00FF0000000000000000000000000");
	left_shift_test("FF0FF00FF00FF00FF00FF", 120, "FF0FF00FF00FF00FF00FF000000000000000000000000000000");
	left_shift_test("FF0FF00FF00FF00FF00FF", 140, "FF0FF00FF00FF00FF00FF00000000000000000000000000000000000");

	left_shift_test("1", 255, "8000000000000000000000000000000000000000000000000000000000000000");
	left_shift_test("1", 256, "0");

	printf("TEST mg_uint256_left_shift_test(): OK\n");
}
