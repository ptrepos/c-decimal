#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arch_priv_uint256.h"

#include "mg_assert.h"

static void convert_test(const char *text)
{
	char strbuf[1024];
	mg_uint256_t v;

	mg_uint256_test_convert(text, &v);

	mg_uint256_test_to_string(&v, strbuf);

	mg_assert(strcmp(text, strbuf) == 0);
}

void mg_uint256_convert_test()
{
	convert_test("1");
	convert_test("10");
	convert_test("99");
	convert_test("999999999999");
	convert_test("13789916531563748961523");
	convert_test("13789916531563748961523857456545123");
	convert_test("1378991653156374896152385745654512315264512345415244111");
	convert_test("99999999999999999999999999999999999999999999999999999999999999999999999999999");
	convert_test("111199999999999999999999999999999999999999999999999999999999999999999999999999");
	convert_test("115792089237316195423570985008687907853269984665640564039457584007913129639935");

	printf("TEST mg_uint256 convert methods: OK\n");
}
