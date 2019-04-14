/**
 * Copyright (C) Takuo Hada 2016
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

#define mgUINT256_WORD_COUNT	(8)
#define mgUINT256_WORD_BITS	(32)

/**
 * 256bit integer implementation for 64bit.
 */
typedef struct mg_uint256
{
	unsigned int word[mgUINT256_WORD_COUNT];
} mg_uint256_t;

/** 
 * value setter function.
 */
int mg_uint256_set_zero(mg_uint256_t *op1);
int mg_uint256_set(mg_uint256_t *op1, uint64_t value);

/** 
 * comparison functions
 */
int mg_uint256_is_zero(const mg_uint256_t *op1);
int mg_uint256_compare(const mg_uint256_t *op1, const mg_uint256_t *op2);

/** 
 * swap pointer
 */
void mg_uint256_swap(mg_uint256_t **a, mg_uint256_t **b);

/**
 * arithmetric functions
 */
int mg_uint256_mul_digits(const mg_uint256_t *op1, int op1_words, const mg_uint256_t *op2, int op2_words, /*out*/mg_uint256_t *ret);
MG_PRIVATE mg_decimal_error mg_uint256_div(/*inout*/mg_uint256_t *op1, const mg_uint256_t *op2, /*out*/mg_uint256_t *quotient);

/**
 * These functions includes for mg_decimal.
 */
bool mg_uint256_is_overflow_int64(const mg_uint256_t *op1);
int64_t mg_uint256_get_int64(mg_uint256_t *op1);
const mg_uint256_t *mg_uint256_get_10eN(int digits);
int mg_uint256_get_digits(const mg_uint256_t *value);

MG_PRIVATE void mg_uint256_test_to_string(const mg_uint256_t *value, char *buf);
MG_PRIVATE void mg_uint256_test_convert(const char *buf, mg_uint256_t *value);
MG_PRIVATE void mg_uint256_test_to_hex_string(const mg_uint256_t *value, char *buf);
MG_PRIVATE void mg_uint256_test_hex_convert(const char *buf, mg_uint256_t *value);

/**
 * 10^N constants table
 * N = 0 ~ 63
 */
extern MG_PRIVATE const mg_uint256_t *MG_UINT256_10eN_TABLE[];


static inline int mg_uint256_set_zero(mg_uint256_t *op1)
{
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		op1->word[i] = 0;
	}
	return 0;
}

static inline int mg_uint256_set(mg_uint256_t *op1, uint64_t value)
{
	op1->word[0] = (uint32_t)value;
	op1->word[1] = (uint32_t)(value >> 32ULL);
	for(int i = 2; i < mgUINT256_WORD_COUNT; i++) {
		op1->word[i] = 0;
	}

	return 0;
}

static inline bool mg_uint256_is_overflow_int64(const mg_uint256_t *op1)
{
	return (op1->word[7] | op1->word[6] | op1->word[5] | op1->word[4] | 
		    op1->word[3] | op1->word[2]) != 0 || 
		   (op1->word[1] & ~0x7FFFFFFFUL) != 0;
}

static inline int64_t mg_uint256_get_int64(mg_uint256_t *op1)
{
	return ((int64_t)(op1->word[1] & 0x7FFFFFFF) << 32ULL) | (int64_t)op1->word[0];
}

static inline int64_t mg_uint256_get_uint64(mg_uint256_t *op1)
{
	return ((int64_t)(op1->word[1]) << 32ULL) | (int64_t)op1->word[0];
}

static inline int32_t mg_uint256_get_int32(mg_uint256_t *op1)
{
	return (int64_t)op1->word[0];
}


static inline void mg_uint256_swap(mg_uint256_t **a, mg_uint256_t **b)
{
	mg_uint256_t *tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline int mg_uint256_is_zero128(const mg_uint256_t *op1)
{
	return (op1->word[3] | op1->word[2] | op1->word[1] | op1->word[0]) == 0;
}

static inline int mg_uint256_is_zero(const mg_uint256_t *op1)
{
	return (op1->word[7] | op1->word[6] | op1->word[5] | op1->word[4] | 
		    op1->word[3] | op1->word[2] | op1->word[1] | op1->word[0]) == 0;
}

static inline bool mg_uint256_is_overflow128(const mg_uint256_t *op1)
{
	return (op1->word[7] | op1->word[6] | op1->word[5] | op1->word[4]) != 0;
}

static inline int mg_uint256_compare(const mg_uint256_t *op1, const mg_uint256_t *op2)
{
	if(op1->word[7] != op2->word[7]) 
		return op1->word[7] < op2->word[7] ? -1: 1;
	if(op1->word[6] != op2->word[6]) 
		return op1->word[6] < op2->word[6] ? -1: 1;
	if(op1->word[5] != op2->word[5]) 
		return op1->word[5] < op2->word[5] ? -1: 1;
	if(op1->word[4] != op2->word[4]) 
		return op1->word[4] < op2->word[4] ? -1: 1;
	if(op1->word[3] != op2->word[3]) 
		return op1->word[3] < op2->word[3] ? -1: 1;
	if(op1->word[2] != op2->word[2]) 
		return op1->word[2] < op2->word[2] ? -1: 1;
	if(op1->word[1] != op2->word[1]) 
		return op1->word[1] < op2->word[1] ? -1: 1;
	if(op1->word[0] != op2->word[0]) 
		return op1->word[0] < op2->word[0] ? -1: 1;
	return 0;
}

static inline void mg_uint256_add128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char c;

	c = 0;
	c = mg_uint32_add(c, op1->word[0], op2->word[0], &op1->word[0]);
	c = mg_uint32_add(c, op1->word[1], op2->word[1], &op1->word[1]);
	c = mg_uint32_add(c, op1->word[2], op2->word[2], &op1->word[2]);
	c = mg_uint32_add(c, op1->word[3], op2->word[3], &op1->word[3]);
}


static inline void mg_uint256_add(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char c;
	
	c = 0;
	c = mg_uint32_add(c, op1->word[0], op2->word[0], &op1->word[0]);
	c = mg_uint32_add(c, op1->word[1], op2->word[1], &op1->word[1]);
	c = mg_uint32_add(c, op1->word[2], op2->word[2], &op1->word[2]);
	c = mg_uint32_add(c, op1->word[3], op2->word[3], &op1->word[3]);
	c = mg_uint32_add(c, op1->word[4], op2->word[4], &op1->word[4]);
	c = mg_uint32_add(c, op1->word[5], op2->word[5], &op1->word[5]);
	c = mg_uint32_add(c, op1->word[6], op2->word[6], &op1->word[6]);
	c = mg_uint32_add(c, op1->word[7], op2->word[7], &op1->word[7]);
}

static inline int mg_uint256_sub128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char b;

	b = 0;
	b = mg_uint32_sub(b, op1->word[0], op2->word[0], &op1->word[0]);
	b = mg_uint32_sub(b, op1->word[1], op2->word[1], &op1->word[1]);
	b = mg_uint32_sub(b, op1->word[2], op2->word[2], &op1->word[2]);
	b = mg_uint32_sub(b, op1->word[3], op2->word[3], &op1->word[3]);

	return b;
}

static inline int mg_uint256_sub(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char b;

	b = 0;
	b = mg_uint32_sub(b, op1->word[0], op2->word[0], &op1->word[0]);
	b = mg_uint32_sub(b, op1->word[1], op2->word[1], &op1->word[1]);
	b = mg_uint32_sub(b, op1->word[2], op2->word[2], &op1->word[2]);
	b = mg_uint32_sub(b, op1->word[3], op2->word[3], &op1->word[3]);
	b = mg_uint32_sub(b, op1->word[4], op2->word[4], &op1->word[4]);
	b = mg_uint32_sub(b, op1->word[5], op2->word[5], &op1->word[5]);
	b = mg_uint32_sub(b, op1->word[6], op2->word[6], &op1->word[6]);
	b = mg_uint32_sub(b, op1->word[7], op2->word[7], &op1->word[7]);

	return b;
}

static inline void mg_uint256_neg128(mg_uint256_t *op1)
{
	unsigned char c;

	op1->word[0] = ~op1->word[0];
	op1->word[1] = ~op1->word[1];
	op1->word[2] = ~op1->word[2];
	op1->word[3] = ~op1->word[3];

	c = 0;
	c = mg_uint32_add(c, op1->word[0], 1, &op1->word[0]);
	c = mg_uint32_add(c, op1->word[1], 0, &op1->word[1]);
	c = mg_uint32_add(c, op1->word[2], 0, &op1->word[2]);
	c = mg_uint32_add(c, op1->word[3], 0, &op1->word[3]);
}


static inline void mg_uint256_neg(mg_uint256_t *op1)
{
	unsigned char c;
	
	op1->word[0] = ~op1->word[0];
	op1->word[1] = ~op1->word[1];
	op1->word[2] = ~op1->word[2];
	op1->word[3] = ~op1->word[3];
	op1->word[4] = ~op1->word[4];
	op1->word[5] = ~op1->word[5];
	op1->word[6] = ~op1->word[6];
	op1->word[7] = ~op1->word[7];

	c = 0;
	c = mg_uint32_add(c, op1->word[0], 1, &op1->word[0]);
	c = mg_uint32_add(c, op1->word[1], 0, &op1->word[1]);
	c = mg_uint32_add(c, op1->word[2], 0, &op1->word[2]);
	c = mg_uint32_add(c, op1->word[3], 0, &op1->word[3]);
	c = mg_uint32_add(c, op1->word[4], 0, &op1->word[4]);
	c = mg_uint32_add(c, op1->word[5], 0, &op1->word[5]);
	c = mg_uint32_add(c, op1->word[6], 0, &op1->word[6]);
	c = mg_uint32_add(c, op1->word[7], 0, &op1->word[7]);
}

static inline int mg_uint256_mul256x64(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	int op1_digits = mgUINT256_WORD_COUNT;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = mgUINT256_WORD_COUNT / 4;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	return mg_uint256_mul_digits(
		op1, op1_digits, 
		op2, op2_digits, 
		/*out*/op1);
}

static inline void mg_uint256_mul128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	int op1_digits = mgUINT256_WORD_COUNT / 2;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = mgUINT256_WORD_COUNT / 2;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	mg_uint256_mul_digits(op1, op1_digits, op2, op2_digits, /*out*/op1);
}

static inline int mg_uint256_mul(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	int op1_digits = mgUINT256_WORD_COUNT;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = mgUINT256_WORD_COUNT;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	return mg_uint256_mul_digits(op1, op1_digits, op2, op2_digits, /*out*/op1);
}


static inline void mg_uint256_or(/*inout*/mg_uint256_t *op1, const mg_uint256_t *op2)
{
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		op1->word[i] = op1->word[i] | op2->word[i];
	}
}

static inline void mg_uint256_left_shift(/*inout*/mg_uint256_t *op1, int op2)
{
	int i;
	mg_uint256_t r;

	int bytes = op2 / mgUINT256_WORD_BITS;
	int bits = op2 % mgUINT256_WORD_BITS;

	if(bytes >= mgUINT256_WORD_COUNT) {
		mg_uint256_set_zero(op1);
	} else if(bytes != 0) {
		if(bits != 0) {
			for(i = 0; i < bytes; i++) {
				r.word[i] = 0;
			}
			r.word[i] = (op1->word[i-bytes] << bits);
			i++;
			for(; i < mgUINT256_WORD_COUNT; i++) {
				r.word[i] = (op1->word[i-bytes] << bits) | (op1->word[i-bytes-1] >> (mgUINT256_WORD_BITS - bits));
			}

			*op1 = r;
		} else {
			for(i = 0; i < bytes; i++) {
				r.word[i] = 0;
			}
			for(; i < mgUINT256_WORD_COUNT; i++) {
				r.word[i] = op1->word[i-bytes];
			}

			*op1 = r;
		}
	} else {
		if(bits != 0) {
			i = 0;
			r.word[i] = (op1->word[i] << bits);
			i++;
			for(; i < mgUINT256_WORD_COUNT; i++) {
				r.word[i] = (op1->word[i-bytes] << bits) | (op1->word[i-bytes-1] >> (mgUINT256_WORD_BITS - bits));
			}

			*op1 = r;
		}
	}
}

static inline void mg_uint256_right_shift(/*inout*/mg_uint256_t *op1, int op2)
{
	int i;
	mg_uint256_t r;

	int bytes = op2 / mgUINT256_WORD_BITS;
	int bits = op2 % mgUINT256_WORD_BITS;

	if(bytes >= mgUINT256_WORD_COUNT) {
		mg_uint256_set_zero(op1);
	} else if(bytes != 0) {
		if(bits != 0) {
			for(i = 0; i < mgUINT256_WORD_COUNT - bytes - 1; i++) {
				r.word[i] = (op1->word[i+bytes] >> bits) | (op1->word[i+bytes+1] << (mgUINT256_WORD_BITS - bits));
			}
			r.word[i] = (op1->word[i+bytes] >> bits);
			i++;
			for(; i < mgUINT256_WORD_COUNT; i++) {
				r.word[i] = 0;
			}

			*op1 = r;
		} else {
			for(i = 0; i < mgUINT256_WORD_COUNT - bytes; i++) {
				op1->word[i] = op1->word[i+bytes];
			}
			for(; i < mgUINT256_WORD_COUNT; i++) {
				op1->word[i] = 0;
			}
		}
	} else {
		if(bits != 0) {
			for(i = 0; i < mgUINT256_WORD_COUNT - 1; i++) {
				r.word[i] = (op1->word[i] >> bits) | (op1->word[i+1] << (mgUINT256_WORD_BITS - bits));
			}
			r.word[i] = (op1->word[i] >> bits);
			i++;
			for(; i < mgUINT256_WORD_COUNT; i++) {
				r.word[i] = 0;
			}

			*op1 = r;
		}
	}
}


static inline const mg_uint256_t *mg_uint256_get_10eN(int digits)
{
	assert(digits < 78);

	return MG_UINT256_10eN_TABLE[digits];
}

static inline void mg_uint256_get_bits(/*inout*/mg_uint256_t *op1, int op2)
{
	int bytes = op2 / mgUINT256_WORD_BITS;
	int bits = op2 % mgUINT256_WORD_BITS;

	for(int i = bytes + (bits > 0 ? 1 : 0); i < mgUINT256_WORD_COUNT; i++) {
		op1->word[i] = 0;
	}

	if(bits != 0) {
		op1->word[bytes] = (op1->word[bytes] << (mgUINT256_WORD_BITS - bits)) >> (mgUINT256_WORD_BITS - bits);
	}
}

static inline int mg_uint256_get_max_bit_index(const mg_uint256_t *value)
{
	for(int i = mgUINT256_WORD_COUNT; i > 0; i--) {
		if(value->word[i-1] != 0) {
			uint64_t n = value->word[i-1];
			for(int j = 0; j < mgUINT256_WORD_BITS; j++) {
				n >>= 1;
				if(n == 0) {
					return (i - 1) * mgUINT256_WORD_BITS + j;
				}
			}
		}
	}
	return -1;
}
