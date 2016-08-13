#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magica/decimal.h>

#include "mg_assert.h"

static void add_loop(const char *text1)
{
	mg_decimal value1, value2;

	mg_assert(mg_decimal_parse_string(text1, &value2) == 0);
	mg_assert(mg_decimal_value_of_int64(1000000000000000ULL, &value1) == 0);

	for(int i = 0; i < 1000000; i++) {
		mg_decimal_add(&value1, &value2, &value1);
	}
}

static void mul_loop(const char *text1)
{
	mg_decimal value1, value2, tmp;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	mg_assert(mg_decimal_parse_string(text1, &value2) == 0);

	for(int i = 0; i < 1000000; i++) {
		mg_decimal_multiply(&value1, &value2, &tmp);
	}
}

static void div_loop(const char *text1, const char *text2)
{
	mg_decimal value1, value2, tmp;

	mg_assert(mg_decimal_parse_string(text1, &value1) == 0);
	mg_assert(mg_decimal_parse_string(text2, &value2) == 0);

	for(int i = 0; i < 1000000; i++) {
		mg_decimal_divide(&value1, &value2, &tmp);
	}
}

void decimal_convert_test();
void decimal_zero_test();
void decimal_compare_test();
void decimal_add_test();
void decimal_subtract_test();
void decimal_negative_test();
void decimal_multiply_test();
void decimal_divide_test();
void decimal_divide_and_modulus_test();
void decimal_modulus_test();
void decimal_error_test();
void decimal_round_test();
void decimal_value_test();
void decimal_abs_test();
void decimal_max_test();
void decimal_min_test();
void decimal_get_binary_test();

void decimal_test()
{
	clock_t tm = clock();
	
	decimal_convert_test();
	decimal_zero_test();
	decimal_compare_test();
	decimal_add_test();
	decimal_subtract_test();
	decimal_negative_test();
	decimal_multiply_test();
	decimal_divide_test();
	decimal_divide_and_modulus_test();
	decimal_modulus_test();
	decimal_error_test();
	decimal_round_test();
	decimal_value_test();
	decimal_abs_test();
	decimal_max_test();
	decimal_min_test();
	decimal_get_binary_test();

	printf("TEST mg_decimal all: %fs\n", (float)(clock() - tm) / CLOCKS_PER_SEC);

#if defined(NDEBUG)
	{
		clock_t tm1 = clock();
		for(int i = 0; i < 10; i++) {
			mul_loop("0.15");
			mul_loop("1.00");
			mul_loop("10.05");
			mul_loop("100000000.3");
			mul_loop("10000000000.5");
		}

		printf("PERFORMANCE TEST mg_decima_multiply(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			add_loop("0.15");
			add_loop("1.00");
			add_loop("10.05");
			add_loop("100000000.3");
			add_loop("10000000000.5");
		}

		printf("PERFORMANCE TEST mg_decimal_add(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}

	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			add_loop("-0.15");
			add_loop("-1.00");
			add_loop("-10.05");
			add_loop("-100000000.3");
			add_loop("-10000000000.5");
		}

		printf("PERFORMANCE TEST mg_decimal_add(subtract): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			div_loop("-0.15", "3");
			div_loop("-1.00", "100000000000000000000");
			div_loop("99999999999999999999999", "99999999999999999999999");
		}

		printf("PERFORMANCE TEST mg_decimal_divide1(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			div_loop("5411355641", "31123");
		}

		printf("PERFORMANCE TEST mg_decimal_divide2(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			div_loop("5411355641", "100000000");
		}

		printf("PERFORMANCE TEST mg_decimal_divide3(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			div_loop("100000000", "100000000");
		}

		printf("PERFORMANCE TEST mg_decimal_divide4(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			div_loop("3333333333", "10");
		}

		printf("PERFORMANCE TEST mg_decimal_divide5(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			div_loop("-100000000.3", "99999999999999999999999999999");
		}

		printf("PERFORMANCE TEST mg_decimal_divide6(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			div_loop("-10.05", "99999999999999999999999");
		}

		printf("PERFORMANCE TEST mg_decimal_divide7(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
	{
		clock_t tm1 = clock();

		for(int i = 0; i < 10; i++) {
			div_loop("-10000000000.5", "99999");
		}

		printf("PERFORMANCE TEST mg_decimal_divide8(): %fs\n", (float)(clock() - tm1) / CLOCKS_PER_SEC);
	}
#endif
}
