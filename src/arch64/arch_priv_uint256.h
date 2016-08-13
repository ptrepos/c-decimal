/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/07/09
 *
 * 256 bit integer for decimal.
 * this source includes decimal implementation.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#include <magica/decimal/decimal.h>

#include "intop.h"

#define MG_UINT256_WORD_COUNT	(4)
#define MG_UINT256_WORD_BITS	(64)

/**
 * 256bit integer implementation for 64bit.
 */
typedef struct mg_uint256
{
	uint64_t word[MG_UINT256_WORD_COUNT];
} mg_uint256;

/** 
 * value setter function.
 */
static inline int mg_uint256_set_zero(mg_uint256 *op1);
static inline int mg_uint256_set(mg_uint256 *op1, uint64_t value);

/** 
 * comparison functions
 */
static inline int mg_uint256_is_zero(const mg_uint256 *op1);
static inline int mg_uint256_compare(const mg_uint256 *op1, const mg_uint256 *op2);

/** 
 * swap pointer
 */
static inline void mg_uint256_swap(mg_uint256 **a, mg_uint256 **b);

/**
 * arithmetric functions
 */
static inline void mg_uint256_add(/*inout*/mg_uint256 *op1, const mg_uint256 *op2);
static inline void mg_uint256_sub(/*inout*/mg_uint256 *op1, const mg_uint256 *op2, /*out*/int *borrow);
static inline void mg_uint256_neg(/*inout*/mg_uint256 *op1);
static inline void mg_uint256_mul128(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret); // multiply for low bits.
static inline void mg_uint256_mul(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret, /*out*/int *overflow);
MG_PRIVATE mg_decimal_error mg_uint256_div(/*inout*/mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *quotient);

static inline void mg_uint256_or(/*inout*/mg_uint256 *op1, const mg_uint256 *op2);
static inline void mg_uint256_left_shift(/*inout*/mg_uint256 *op1, int op2);
static inline void mg_uint256_right_shift(/*inout*/mg_uint256 *op1, int op2);

/**
 * These functions includes for mg_decimal.
 */
static inline bool mg_uint256_is_overflow_int64(const mg_uint256 *op1);
static inline int64_t mg_uint256_get_int64(mg_uint256 *op1);
static inline const mg_uint256 *mg_uint256_get_10eN(int digits);
MG_PRIVATE int mg_uint256_get_digits(const mg_uint256 *value);

static inline void mg_uint256_modulus_by_scale(/*inout*/mg_uint256 *op1, int op2);
static inline int mg_uint256_get_max_bit_index(const mg_uint256 *value);

MG_PRIVATE void mg_uint256_test_to_string(const mg_uint256 *value, char *buf);
MG_PRIVATE void mg_uint256_test_convert(const char *buf, mg_uint256 *value);
MG_PRIVATE void mg_uint256_test_to_hex_string(const mg_uint256 *value, char *buf);
MG_PRIVATE void mg_uint256_test_hex_convert(const char *buf, mg_uint256 *value);


/**
 * 10^N constants table
 * N = 0 ~ 63
 */
extern MG_PRIVATE const mg_uint256 *MG_UINT256_10eN_TABLE[];

static inline int mg_uint256_set_zero(mg_uint256 *op1)
{
	for(int i = 0; i < MG_UINT256_WORD_COUNT; i++) {
		op1->word[i] = 0;
	}
	return 0;
}

static inline int mg_uint256_set(mg_uint256 *op1, uint64_t value)
{
	op1->word[0] = value;
	for(int i = 1; i < MG_UINT256_WORD_COUNT; i++) {
		op1->word[i] = 0;
	}

	return 0;
}

static inline bool mg_uint256_is_overflow_int64(const mg_uint256 *op1)
{
	return (op1->word[3] | op1->word[2] | op1->word[1]) != 0 || 
		   (op1->word[0] & ~0x7FFFFFFFFFFFFFFFULL) != 0;
}

static inline int64_t mg_uint256_get_int64(mg_uint256 *op1)
{
	return op1->word[0] & 0x7FFFFFFFFFFFFFFFULL;
}

static inline uint64_t mg_uint256_get_uint64(mg_uint256 *op1)
{
	return op1->word[0];
}

static inline void mg_uint256_swap(mg_uint256 **a, mg_uint256 **b)
{
	mg_uint256 *tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline int mg_uint256_is_zero(const mg_uint256 *op1)
{
	return (op1->word[3] | op1->word[2] | op1->word[1] | op1->word[0]) == 0;
}

static inline int mg_uint256_compare(const mg_uint256 *op1, const mg_uint256 *op2)
{
	for(int i = MG_UINT256_WORD_COUNT; i > 0; i--) {
		if(op1->word[i-1] != op2->word[i-1]) 
			return op1->word[i-1] < op2->word[i-1] ? -1: 1;
	}
	return 0;
}

static inline void mg_uint256_add128(mg_uint256 *op1, const mg_uint256 *op2)
{
	unsigned char c;

	c = 0;
	c = mg_uint64_add(c, op1->word[0], op2->word[0], &op1->word[0]);
	c = mg_uint64_add(c, op1->word[1], op2->word[1], &op1->word[1]);
}

static inline void mg_uint256_add(mg_uint256 *op1, const mg_uint256 *op2)
{
	unsigned char c;
	
	c = 0;
	c = mg_uint64_add(c, op1->word[0], op2->word[0], &op1->word[0]);
	c = mg_uint64_add(c, op1->word[1], op2->word[1], &op1->word[1]);
	c = mg_uint64_add(c, op1->word[2], op2->word[2], &op1->word[2]);
	c = mg_uint64_add(c, op1->word[3], op2->word[3], &op1->word[3]);
}

static inline void mg_uint256_sub128(mg_uint256 *op1, const mg_uint256 *op2, /*out*/int *borrow)
{
	unsigned char b;

	b = 0;
	b = mg_uint64_sub(b, op1->word[0], op2->word[0], &op1->word[0]);
	b = mg_uint64_sub(b, op1->word[1], op2->word[1], &op1->word[1]);

	*borrow = b;
}

static inline void mg_uint256_sub(mg_uint256 *op1, const mg_uint256 *op2, /*out*/int *borrow)
{
	unsigned char b;

	b = 0;
	b = mg_uint64_sub(b, op1->word[0], op2->word[0], &op1->word[0]);
	b = mg_uint64_sub(b, op1->word[1], op2->word[1], &op1->word[1]);
	b = mg_uint64_sub(b, op1->word[2], op2->word[2], &op1->word[2]);
	b = mg_uint64_sub(b, op1->word[3], op2->word[3], &op1->word[3]);

	*borrow = b;
}

static inline void mg_uint256_neg128(mg_uint256 *op1)
{
	unsigned char c;

	op1->word[0] = ~op1->word[0];
	op1->word[1] = ~op1->word[1];

	c = 0;
	c = mg_uint64_add(c, op1->word[0], 1, &op1->word[0]);
	c = mg_uint64_add(c, op1->word[1], 0, &op1->word[1]);
}


static inline void mg_uint256_neg(mg_uint256 *op1)
{
	unsigned char c;
	
	op1->word[0] = ~op1->word[0];
	op1->word[1] = ~op1->word[1];
	op1->word[2] = ~op1->word[2];
	op1->word[3] = ~op1->word[3];

	c = 0;
	c = mg_uint64_add(c, op1->word[0], 1, &op1->word[0]);
	c = mg_uint64_add(c, op1->word[1], 0, &op1->word[1]);
	c = mg_uint64_add(c, op1->word[2], 0, &op1->word[2]);
	c = mg_uint64_add(c, op1->word[3], 0, &op1->word[3]);
}

static inline void mg_uint256_mul_with_words(const mg_uint256 *op1, int op1_words, const mg_uint256 *op2, int op2_words, /*out*/mg_uint256 *ret, /*out*/int *overflow)
{
	unsigned carry, carry2;
	uint64_t lo, hi;

	*overflow = 0;
	mg_uint256_set_zero(ret);

	for(int i = 0; i < op1_words; i++) {
		carry2 = 0;
		for(int j = 0; j < op2_words; j++) {
			int k = i + j;
			if(k >= MG_UINT256_WORD_COUNT) {
				if(!((op1->word[i] == 0 || op2->word[j] == 0) && carry2 == 0)) {
					*overflow = 1;
					return;
				}
				break;
			}

			lo = mg_uint64_mul(op1->word[i], op2->word[j], &hi);

			carry = mg_uint64_add(0, ret->word[k], lo, &ret->word[k]);

			k++;
			if(k >= MG_UINT256_WORD_COUNT) {
				if(!(hi == 0 && carry == 0)) {
					*overflow = 1;
					return;
				}
				continue;
			}
			carry = mg_uint64_add(carry, ret->word[k], hi, &ret->word[k]);

			k++;
			if(k >= MG_UINT256_WORD_COUNT) {
				if(!(carry2 == 0 && carry == 0)) {
					*overflow = 1;
					return;
				}
				continue;
			}
			carry2 = mg_uint64_add(carry, ret->word[k], carry2, &ret->word[k]);
		}
	}
}

static inline void mg_uint256_mul128(const mg_uint256 *op1, const mg_uint256 *op2, mg_uint256 *ret)
{
	unsigned c;
	uint64_t lo, hi;
	
	lo = mg_uint64_mul(op1->word[0], op2->word[0], &hi);
	
	ret->word[0] = lo;
	ret->word[1] = hi;
	
	lo = mg_uint64_mul(op1->word[1], op2->word[1], &hi);
	
	ret->word[2] = lo;
	ret->word[3] = hi;

	lo = mg_uint64_mul(op1->word[0], op2->word[1], &hi);
	
	c = 0;
	c = mg_uint64_add(c, ret->word[1], lo, &ret->word[1]);
	c = mg_uint64_add(c, ret->word[2], hi, &ret->word[2]);
	c = mg_uint64_add(c, ret->word[3], 0, &ret->word[3]);

	lo = mg_uint64_mul(op1->word[1], op2->word[0], &hi);

	c = 0;
	c = mg_uint64_add(c, ret->word[1], lo, &ret->word[1]);
	c = mg_uint64_add(c, ret->word[2], hi, &ret->word[2]);
	c = mg_uint64_add(c, ret->word[3], 0, &ret->word[3]);
}

static inline void mg_uint256_mul(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret, /*out*/int *overflow)
{
	int op1_digits = MG_UINT256_WORD_COUNT;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = MG_UINT256_WORD_COUNT;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	mg_uint256_mul_with_words(op1, op1_digits, op2, op2_digits, /*out*/ret, /*out*/overflow);
}

static inline void mg_uint256_or(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	for(int i = 0; i < MG_UINT256_WORD_COUNT; i++) {
		op1->word[i] = op1->word[i] | op2->word[i];
	}
}

static inline void mg_uint256_left_shift(/*inout*/mg_uint256 *op1, int op2)
{
	int i;
	mg_uint256 r;

	int bytes = op2 / MG_UINT256_WORD_BITS;
	int bits = op2 % MG_UINT256_WORD_BITS;

	if(bytes >= MG_UINT256_WORD_COUNT) {
		mg_uint256_set_zero(op1);
	} else if(bytes != 0) {
		if(bits != 0) {
			for(i = 0; i < bytes; i++) {
				r.word[i] = 0;
			}
			r.word[i] = (op1->word[i-bytes] << bits);
			i++;
			for(; i < MG_UINT256_WORD_COUNT; i++) {
				r.word[i] = (op1->word[i-bytes] << bits) | (op1->word[i-bytes-1] >> (MG_UINT256_WORD_BITS - bits));
			}

			*op1 = r;
		} else {
			for(i = 0; i < bytes; i++) {
				r.word[i] = 0;
			}
			for(; i < MG_UINT256_WORD_COUNT; i++) {
				r.word[i] = op1->word[i-bytes];
			}

			*op1 = r;
		}
	} else {
		if(bits != 0) {
			i = 0;
			r.word[i] = (op1->word[i] << bits);
			i++;
			for(; i < MG_UINT256_WORD_COUNT; i++) {
				r.word[i] = (op1->word[i-bytes] << bits) | (op1->word[i-bytes-1] >> (MG_UINT256_WORD_BITS - bits));
			}

			*op1 = r;
		}
	}
}

static inline void mg_uint256_right_shift(/*inout*/mg_uint256 *op1, int op2)
{
	int i;
	mg_uint256 r;

	int bytes = op2 / MG_UINT256_WORD_BITS;
	int bits = op2 % MG_UINT256_WORD_BITS;

	if(bytes >= MG_UINT256_WORD_COUNT) {
		mg_uint256_set_zero(op1);
	} else if(bytes != 0) {
		if(bits != 0) {
			for(i = 0; i < MG_UINT256_WORD_COUNT - bytes - 1; i++) {
				r.word[i] = (op1->word[i+bytes] >> bits) | (op1->word[i+bytes+1] << (MG_UINT256_WORD_BITS - bits));
			}
			r.word[i] = (op1->word[i+bytes] >> bits);
			i++;
			for(; i < MG_UINT256_WORD_COUNT; i++) {
				r.word[i] = 0;
			}

			*op1 = r;
		} else {
			for(i = 0; i < MG_UINT256_WORD_COUNT - bytes; i++) {
				op1->word[i] = op1->word[i+bytes];
			}
			for(; i < MG_UINT256_WORD_COUNT; i++) {
				op1->word[i] = 0;
			}
		}
	} else {
		if(bits != 0) {
			for(i = 0; i < MG_UINT256_WORD_COUNT - 1; i++) {
				r.word[i] = (op1->word[i] >> bits) | (op1->word[i+1] << (MG_UINT256_WORD_BITS - bits));
			}
			r.word[i] = (op1->word[i] >> bits);
			i++;
			for(; i < MG_UINT256_WORD_COUNT; i++) {
				r.word[i] = 0;
			}

			*op1 = r;
		}
	}
}

static inline const mg_uint256 *mg_uint256_get_10eN(int digits)
{
	assert(0 <= digits && digits < 78);

	return MG_UINT256_10eN_TABLE[digits];
}

static inline void mg_uint256_modulus_by_scale(/*inout*/mg_uint256 *op1, int op2)
{
	int bytes = op2 / MG_UINT256_WORD_BITS;
	int bits = op2 % MG_UINT256_WORD_BITS;

	for(int i = bytes + 1; i < MG_UINT256_WORD_COUNT; i++) {
		op1->word[i] = 0;
	}

	if(bits != 0) {
		op1->word[bytes] = (op1->word[bytes] << (MG_UINT256_WORD_BITS - bits)) >> (MG_UINT256_WORD_BITS - bits);
	}
}

static inline int mg_uint256_get_max_bit_index(const mg_uint256 *value)
{
	for(int i = MG_UINT256_WORD_COUNT; i > 0; i--) {
		if(value->word[i-1] != 0) {
			uint64_t n = value->word[i-1];
			for(int j = 0; j < MG_UINT256_WORD_BITS; j++) {
				n >>= 1;
				if(n == 0) {
					return (i - 1) * MG_UINT256_WORD_BITS + j;
				}
			}
		}
	}
	return -1;
}
