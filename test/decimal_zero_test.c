#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void zero_test()
{
	char strbuf[1000];
	int size;
	mg_decimal value1;

	mg_decimal_zero(&value1);

	mg_assert(mg_decimal_to_string(&value1, strbuf, 1000, &size) == 0);

	mg_assert(strcmp(strbuf, "0") == 0);
}

void decimal_zero_test()
{
	zero_test();

	printf("TEST mg_decimal_zero(): OK\n");
}
