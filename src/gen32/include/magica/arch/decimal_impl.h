/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2011/03/15 
 *
 * 128 bit decimal implements.
 */
#pragma once

#include <magica/decimal/decimal.h>

#include <magica/arch/uint256.h>
#include <decimal_impl.h>

#define WORD_COUNT			(sizeof(mg_decimal_t) / sizeof(uint32_t))
#define INFO_INDEX			(WORD_COUNT-1)

#define SCALE_MASK			(0x7F000000)
#define SCALE_BITINDEX		(24)
#define SIGN_MASK			(0x80000000)
#define SIGN_BITINDEX		(31)
#define FRACTION_MASK		(0x00FFFFFF)
#define FRACTION_BITINDEX	(0)

static inline int _decimal_is_overflow(const mg_uint256_t *value)
{
	return mg_uint256_compare(value, mg_uint256_get_10eN(DIGIT_MAX)) >= 0;
}

static inline uint32_t _decimal_get_bits(uint32_t dest, uint32_t mask, int bitindex)
{
	return (dest & mask) >> bitindex;
}

static inline uint32_t _deciaml_set_bits(uint32_t dest, uint32_t value, uint32_t mask, int bitindex)
{
	return (dest & ~mask) | ((value << bitindex) & mask);
}

static inline int _decimal_get_sign(const mg_decimal_t *value)
{
	return (int)_decimal_get_bits(
		value->u.b32.word3, SIGN_MASK, SIGN_BITINDEX);
}

static inline void _decimal_set_sign(mg_decimal_t *value, int sign)
{
	value->u.b32.word3 = _deciaml_set_bits(
		value->u.b32.word3,
		sign,
		SIGN_MASK,
		SIGN_BITINDEX);
}

static inline int _decimal_get_scale(const mg_decimal_t *value)
{
	int bits = (int)_decimal_get_bits(
		value->u.b32.word3, SCALE_MASK, SCALE_BITINDEX);
	return (bits & SCALE_SIGNEXPAND) == 0 ? bits: (short)(bits | SCALE_SIGNEXPAND);
}

static inline void _decimal_get_fraction(const mg_decimal_t *value, mg_uint256_t *buf)
{
	buf->word[0] = value->u.b32.word0;
	buf->word[1] = value->u.b32.word1;
	buf->word[2] = value->u.b32.word2;
	buf->word[3] = (uint32_t)_decimal_get_bits(value->u.b32.word3, FRACTION_MASK, FRACTION_BITINDEX);
	buf->word[4] = 0;
	buf->word[5] = 0;
	buf->word[6] = 0;
	buf->word[7] = 0;
}

static inline mg_error_t _decimal_set(mg_decimal_t *value, int sign, int scale, const mg_uint256_t *fraction)
{
	mg_error_t err;

	assert(sign == 0 || sign == 1);

	if(!(SCALE_MIN <= scale && scale <= SCALE_MAX)) {
		err = mgE_OVERFLOW;
		goto _ERROR;
	}

	if(_decimal_is_overflow(fraction)) {
		err = mgE_OVERFLOW;
		goto _ERROR;
	}

	value->u.b32.word0 = fraction->word[0];
	value->u.b32.word1 = fraction->word[1];
	value->u.b32.word2 = fraction->word[2];
	value->u.b32.word3 =
		((sign << SIGN_BITINDEX) & SIGN_MASK) | 
		((scale << SCALE_BITINDEX) & SCALE_MASK) | 
		((fraction->word[3] << FRACTION_BITINDEX) & FRACTION_MASK);

	return 0;
_ERROR:
	return err;
}

static inline void _decimal_set2(mg_decimal_t *value, int sign, int scale, const mg_uint256_t *fraction)
{
	assert(_decimal_is_overflow(fraction) == 0);
	assert(SCALE_MIN <= scale && scale <= SCALE_MAX);
	assert(sign == 0 || sign == 1);

	value->u.b32.word0 = fraction->word[0];
	value->u.b32.word1 = fraction->word[1];
	value->u.b32.word2 = fraction->word[2];
	value->u.b32.word3 =
		((sign << SIGN_BITINDEX) & SIGN_MASK) | 
		((scale << SCALE_BITINDEX) & SCALE_MASK) | 
		((fraction->word[3] << FRACTION_BITINDEX) & FRACTION_MASK);

	return ;
}
