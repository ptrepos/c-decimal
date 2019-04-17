#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void error_test()
{
	mg_decimal_t v1, v2, v3, v4;

	mg_assert(mg_decimal_parse_string("519229685853482762853049632922009699", &v1) == 0);
	mg_assert(mg_decimal_parse_string("519229685853482762853049632922009699", &v2) == 0);
	mg_assert(mg_decimal_add(&v1, &v2, /*out*/&v3) == mgE_OVERFLOW);

	mg_assert(mg_decimal_parse_string("999999999999999999999999999999999999", &v1) == 0);
	mg_assert(mg_decimal_parse_string("1", &v2) == 0);
	mg_assert(mg_decimal_add(&v1, &v2, /*out*/&v3) == mgE_OVERFLOW);

	mg_assert(mg_decimal_parse_string("999999999999999999999999999999999999", &v1) == 0);
	mg_assert(mg_decimal_parse_string("1", &v2) == 0);
	mg_assert(mg_decimal_add(&v2, &v1, /*out*/&v3) == mgE_OVERFLOW);

	mg_assert(mg_decimal_parse_string("999999999999999999999999999999999999", &v1) == 0);
	mg_assert(mg_decimal_parse_string("-1", &v2) == 0);
	mg_assert(mg_decimal_subtract(&v1, &v2, /*out*/&v3) == mgE_OVERFLOW);

	mg_assert(mg_decimal_parse_string("519229685853482762853049632922009699", &v1) == 0);
	mg_assert(mg_decimal_parse_string("-519229685853482762853049632922009699", &v2) == 0);
	mg_assert(mg_decimal_subtract(&v1, &v2, /*out*/&v3) == mgE_OVERFLOW);

	mg_assert(mg_decimal_parse_string("999999999999999999999999999999999999", &v1) == 0);
	mg_assert(mg_decimal_parse_string("2", &v2) == 0);
	mg_assert(mg_decimal_multiply(&v1, &v2, /*out*/&v3) == mgE_OVERFLOW);

	mg_assert(mg_decimal_parse_string("5192296858534827628530496329220096", &v1) == 0);
	mg_assert(mg_decimal_parse_string("5192296858534827628530496329220096", &v2) == 0);
	mg_assert(mg_decimal_multiply(&v1, &v2, /*out*/&v3) == mgE_OVERFLOW);

	mg_assert(mg_decimal_parse_string("1000000000000000000", &v1) == 0);
	mg_assert(mg_decimal_parse_string("1000000000000000000", &v2) == 0);
	mg_assert(mg_decimal_multiply(&v1, &v2, /*out*/&v3) == mgE_OVERFLOW);

	mg_assert(mg_decimal_parse_string("9999999999999999999999999999999999", &v1) == 0);
	mg_assert(mg_decimal_parse_string("0", &v2) == 0);
	mg_assert(mg_decimal_divide(&v1, &v2, /*out*/&v3) == mgE_ZERODIVIDE);

	mg_assert(mg_decimal_parse_string("9999999999999999999999999999999999", &v1) == 0);
	mg_assert(mg_decimal_parse_string("0", &v2) == 0);
	mg_assert(mg_decimal_divide_and_modulus(&v1, &v2, /*out*/&v3, /*out*/&v4) == mgE_ZERODIVIDE);

	mg_assert(mg_decimal_parse_string("999999999999999999999999999999999999", &v1) == 0);
	mg_assert(mg_decimal_parse_string("0.1", &v2) == 0);
	mg_assert(mg_decimal_divide_and_modulus(&v1, &v2, /*out*/&v3, /*out*/&v4) == mgE_OVERFLOW);
}

void decimal_error_test()
{
	clock_t tm = clock();

	error_test();
	
	printf("TEST mg_decimal error pattern: OK\n");

}
