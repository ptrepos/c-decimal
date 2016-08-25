/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/07/09
 *
 * 256 bit integer for decimal.
 * this source includes decimal implementation.
 */
#pragma once

#include <stdint.h>
#include <memory.h>
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
int mg_uint256_set_zero(mg_uint256 *op1);
int mg_uint256_set(mg_uint256 *op1, uint64_t value);

/** 
 * comparison functions
 */
int mg_uint256_is_zero(const mg_uint256 *op1);
int mg_uint256_compare(const mg_uint256 *op1, const mg_uint256 *op2);

/** 
 * swap pointer
 */
void mg_uint256_swap(mg_uint256 **a, mg_uint256 **b);

/**
 * arithmetric functions
 */
void mg_uint256_add(/*inout*/mg_uint256 *op1, const mg_uint256 *op2);
void mg_uint256_sub(/*inout*/mg_uint256 *op1, const mg_uint256 *op2, /*out*/int *borrow);
void mg_uint256_neg(/*inout*/mg_uint256 *op1);
int mg_uint256_mul_words(const mg_uint256 *op1, int op1_words, const mg_uint256 *op2, int op2_words, /*out*/mg_uint256 *ret);
void mg_uint256_mul128(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret); // multiply for low bits.
int mg_uint256_mul256x64(const mg_uint256 *op1, const mg_uint256 *op2, mg_uint256 *ret);
int mg_uint256_mul(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret);
MG_PRIVATE mg_decimal_error mg_uint256_div(/*inout*/mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *quotient);

void mg_uint256_or(/*inout*/mg_uint256 *op1, const mg_uint256 *op2);
void mg_uint256_left_shift(/*inout*/mg_uint256 *op1, int op2);
void mg_uint256_right_shift(/*inout*/mg_uint256 *op1, int op2);

/**
 * These functions includes for mg_decimal.
 */
bool mg_uint256_is_overflow_int64(const mg_uint256 *op1);
int64_t mg_uint256_get_int64(mg_uint256 *op1);
const mg_uint256 *mg_uint256_get_10eN(int digits);
MG_PRIVATE int mg_uint256_get_digits(const mg_uint256 *value);

void mg_uint256_get_bits(/*inout*/mg_uint256 *op1, int op2);
int mg_uint256_get_max_bit_index(const mg_uint256 *value);

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
	if(op1->word[3] != op2->word[3]) 
		return op1->word[3] < op2->word[3] ? -1: 1;
	if(op1->word[2] != op2->word[2]) 
		return op1->word[2] < op2->word[2] ? -1: 1;
	if (op1->word[1] != op2->word[1])
		return op1->word[1] < op2->word[1] ? -1 : 1;
	if (op1->word[0] != op2->word[0])
		return op1->word[0] < op2->word[0] ? -1 : 1;
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

static inline int mg_uint256_mul256x64(
		const mg_uint256 *op1, 
		const mg_uint256 *op2, 
		mg_uint256 *ret)
{
	unsigned char carry, carry2;
	uint64_t hi, lo;

	mg_uint256_set_zero(ret);

	carry2 = 0;

	lo = mg_uint64_mul(op1->word[0], op2->word[0], &hi);

	carry = mg_uint64_add(0, ret->word[0], lo, &ret->word[0]);
	carry = mg_uint64_add(carry, ret->word[1], hi, &ret->word[1]);
	carry2 = mg_uint64_add(carry, ret->word[2], carry2, &ret->word[2]);

	lo = mg_uint64_mul(op1->word[1], op2->word[0], &hi);

	carry = mg_uint64_add(0, ret->word[1], lo, &ret->word[1]);
	carry = mg_uint64_add(carry, ret->word[2], hi, &ret->word[2]);
	carry2 = mg_uint64_add(carry, ret->word[3], carry2, &ret->word[3]);

	lo = mg_uint64_mul(op1->word[2], op2->word[0], &hi);

	carry = mg_uint64_add(0, ret->word[2], lo, &ret->word[2]);
	carry = mg_uint64_add(carry, ret->word[3], hi, &ret->word[3]);
	if(carry != 0 || carry2 != 0) {
		return 1;
	}

	lo = mg_uint64_mul(op1->word[3], op2->word[0], &hi);

	carry = mg_uint64_add(0, ret->word[3], lo, &ret->word[3]);
	if(carry != 0 || hi != 0) {
		return 1;
	}

	return 0;
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

static inline int mg_uint256_mul(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	int op1_digits = MG_UINT256_WORD_COUNT;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = MG_UINT256_WORD_COUNT;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	return mg_uint256_mul_words(op1, op1_digits, op2, op2_digits, /*out*/ret);
}

static inline void mg_uint256_or(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	for(int i = 0; i < MG_UINT256_WORD_COUNT; i++) {
		op1->word[i] = op1->word[i] | op2->word[i];
	}
}

static inline void mg_uint256_left_shift(mg_uint256 *op1, int op2)
{
	uint64_t buf[MG_UINT256_WORD_COUNT * 2] = { 0 };

	int words = op2 / 64;
	int bits = op2 % 64;

	if (bits != 0) {
		buf[0 + words] = (op1->word[0] << bits);
		buf[1 + words] = (op1->word[0] >> (64 - bits)) | (op1->word[1] << bits);
		buf[2 + words] = (op1->word[1] >> (64 - bits)) | (op1->word[2] << bits);
		buf[3 + words] = (op1->word[2] >> (64 - bits)) | (op1->word[3] << bits);
		buf[4 + words] = (op1->word[3] >> (64 - bits));
	}
	else {
		buf[0 + words] = op1->word[0];
		buf[1 + words] = op1->word[1];
		buf[2 + words] = op1->word[2];
		buf[3 + words] = op1->word[3];
	}

	op1->word[0] = buf[0];
	op1->word[1] = buf[1];
	op1->word[2] = buf[2];
	op1->word[3] = buf[3];
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

static inline void mg_uint256_right_shift_1(mg_uint256 *op1)
{
	mg_uint256 buf;
	buf.word[0] = (op1->word[0] >> 1) | (op1->word[1] << (64 - 1));
	buf.word[1] = (op1->word[1] >> 1) | (op1->word[2] << (64 - 1));
	buf.word[2] = (op1->word[2] >> 1) | (op1->word[3] << (64 - 1));
	buf.word[3] = (op1->word[3] >> 1);

	*op1 = buf;
}

static inline const mg_uint256 *mg_uint256_get_10eN(int digits)
{
	assert(0 <= digits && digits < 78);

	return MG_UINT256_10eN_TABLE[digits];
}

static inline void mg_uint256_get_bits(/*inout*/mg_uint256 *op1, int op2)
{
	int bytes = op2 / MG_UINT256_WORD_BITS;
	int bits = op2 % MG_UINT256_WORD_BITS;

	for(int i = bytes + (bits > 0 ? 1 : 0); i < MG_UINT256_WORD_COUNT; i++) {
		op1->word[i] = 0;
	}

	if(bits != 0) {
		op1->word[bytes] = (op1->word[bytes] << (MG_UINT256_WORD_BITS - bits)) >> (MG_UINT256_WORD_BITS - bits);
	}
}

static inline int mg_uint64_get_max_bit_index(uint64_t value)
{
	int n = 0;

	if ((value & 0xFFFFFFFF00000000ULL) != 0) {
		n += 32;
		value >>= 32;
	}
	if ((value & 0xFFFF0000ULL) != 0) {
		n += 16;
		value >>= 16;
	}
	if ((value & 0xFF00ULL) != 0) {
		n += 8;
		value >>= 8;
	}
	if ((value & 0xF0ULL) != 0) {
		n += 4;
		value >>= 4;
	}
	if ((value & 0xCULL) != 0) {
		n += 2;
		value >>= 2;
	}
	if ((value & 0x2ULL) != 0) {
		n += 1;
		value >>= 1;
	}
	if ((value & 0x1ULL) != 0) {
		n += 0;
		value >>= 1;
	}

	return n;
}

static inline int mg_uint256_get_max_bit_index(const mg_uint256 *value)
{
	for(int i = MG_UINT256_WORD_COUNT; i > 0; i--) {
		if(value->word[i-1] != 0) {
			return (i - 1) * MG_UINT256_WORD_BITS + mg_uint64_get_max_bit_index(value->word[i - 1]);
		}
	}
	return -1;
}

static inline void mg_uint256_set_bit(mg_uint256 *op1, int bit)
{
	int words = bit / 64;
	int bits = bit % 64;

	op1->word[words] |= (uint64_t)1ULL << (uint64_t)bits;
}
