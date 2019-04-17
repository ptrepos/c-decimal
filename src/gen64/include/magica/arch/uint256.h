/**
 * Copyright (C) Takuo Hada 2015-2019
 * @author t.hada 2016/07/09
 *
 * 256 bit integer for decimal.
 * this source includes decimal implementation.
 */
#pragma once

#include <magica/decimal/decimal.h>

#include <magica/arch/def.h>
#include <magica/arch/uint256_add.h>
#include <magica/arch/uint256_sub.h>
#include <magica/arch/uint256_neg.h>
#include <magica/arch/uint256_less_than.h>

#include <magica/arch/uint256_add128.h>
#include <magica/arch/uint256_sub128.h>
#include <magica/arch/uint256_neg128.h>
#include <magica/arch/uint256_mul256x64.h>

#include <magica/arch/uint256_add_ull.h>
/**
 * multiply lower 128bits
 */
void mg_uint256_mul128(mg_uint256_t *op1, const mg_uint256_t *op2);

/**
 * overflow checked multiple for digits of word.
 */
bool mg_uint256_mul_digits(const mg_uint256_t *op1, int op1_digits, const mg_uint256_t *op2, int op2_digits, mg_uint256_t *result);

/**
 * overflow checked 256bits multiple.
 */
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

/**
 * uint256 divide
 */
mg_error_t mg_uint256_div(mg_uint256_t *op1, const mg_uint256_t *op2, mg_uint256_t *quotient);

/**
 * get decimal digits.
 */
MG_PRIVATE int mg_uint256_get_digits(const mg_uint256_t *value);

/**
 * 10^N constants table
 * N = 0 ~ 77
 */
extern MG_PRIVATE const mg_uint256_t *MG_UINT256_10eN_TABLE[];

/**
 * get 10^N constants table
 * N = 0 ~ 77
 */
static inline const mg_uint256_t *mg_uint256_get_10eN(int digits)
{
	assert(0 <= digits && digits < 78);

	return MG_UINT256_10eN_TABLE[digits];
}

/**
 * set zero.
 */
static inline void mg_uint256_set_zero(mg_uint256_t *op1)
{
	op1->word[0] = 0;
	op1->word[1] = 0;
	op1->word[2] = 0;
	op1->word[3] = 0;
}

/**
 * set value of unsigned long long
 */
static inline int mg_uint256_set(mg_uint256_t *op1, unsigned long long value)
{
	op1->word[0] = value;
	op1->word[1] = 0;
	op1->word[2] = 0;
	op1->word[3] = 0;

	return 0;
}

/**
 * swap two pointer.
 */
static inline void mg_uint256_swap(mg_uint256_t **a, mg_uint256_t **b)
{
	mg_uint256_t *tmp = *a;
	*a = *b;
	*b = tmp;
}

/**
 * get value of unsigned long long
 */
static inline unsigned long long mg_uint256_get_uint64(mg_uint256_t *op1)
{
	return op1->word[0];
}

/**
 * get value of long long
 */
static inline long long mg_uint256_get_int64(mg_uint256_t *op1)
{
	return op1->word[0] & 0x7FFFFFFFFFFFFFFFULL;
}

/**
 * op1 is zero.
 */
static inline bool mg_uint256_is_zero(const mg_uint256_t *op1)
{
	return (op1->word[3] | op1->word[2] | op1->word[1] | op1->word[0]) == 0;
}

/**
 * op1 is zero for lower 128bits.
 */
static inline bool mg_uint256_is_zero128(const mg_uint256_t *op1)
{
	return (op1->word[1] | op1->word[0]) == 0;
}

/**
 * compare op1 and op2.
 */
static inline int mg_uint256_compare(const mg_uint256_t *op1, const mg_uint256_t *op2)
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

/**
 * use upper 128bits.
 */
static inline bool mg_uint256_is_overflow128(const mg_uint256_t *op1)
{
	return (op1->word[2] | op1->word[3]) != 0;
}

/**
 * bit and
 */
static inline void mg_uint256_and(/*inout*/mg_uint256_t *op1, const mg_uint256_t *op2)
{
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		op1->word[i] = op1->word[i] & op2->word[i];
	}
}

/**
 * bit or
 */
static inline void mg_uint256_or(/*inout*/mg_uint256_t *op1, const mg_uint256_t *op2)
{
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		op1->word[i] = op1->word[i] | op2->word[i];
	}
}

/**
 * bit xor
 */
static inline void mg_uint256_xor(/*inout*/mg_uint256_t *op1, const mg_uint256_t *op2)
{
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		op1->word[i] = op1->word[i] ^ op2->word[i];
	}
}

/**
 * bit not
 */
static inline void mg_uint256_not(/*inout*/mg_uint256_t *op1)
{
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		op1->word[i] = ~op1->word[i];
	}
}

/**
 * bit left shift.
 */
static inline void mg_uint256_left_shift(mg_uint256_t *op1, int op2)
{
	uint64_t buf[mgUINT256_WORD_COUNT * 2] = { 0 };

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

/**
 * bit right shift.
 */
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

/**
 * get instruction bits.
 */
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

/**
 * utility for bit scan head.
 */
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

/**
 * bit scan head
 */
static inline int mg_uint256_get_max_bit_index(const mg_uint256_t *value)
{
	for(int i = mgUINT256_WORD_COUNT; i > 0; i--) {
		if(value->word[i-1] != 0) {
			return (i - 1) * mgUINT256_WORD_BITS + mg_uint64_get_max_bit_index(value->word[i - 1]);
		}
	}
	return -1;
}

/**
 * set instruction bit.
 */
static inline void mg_uint256_set_bit(mg_uint256_t *op1, int bit)
{
	int words = bit / 64;
	int bits = bit % 64;

	op1->word[words] |= (unsigned long long)1ULL << (uint64_t)bits;
}


MG_PRIVATE void mg_uint256_test_to_string(const mg_uint256_t *value, char *buf);
MG_PRIVATE void mg_uint256_test_convert(const char *buf, mg_uint256_t *value);
MG_PRIVATE void mg_uint256_test_to_hex_string(const mg_uint256_t *value, char *buf);
MG_PRIVATE void mg_uint256_test_hex_convert(const char *buf, mg_uint256_t *value);