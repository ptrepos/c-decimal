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

#define WORD_COUNT			(sizeof(mg_decimal_t) / sizeof(unsigned long long))
#define INFO_INDEX			(WORD_COUNT-1)

#define SCALE_MASK			(0x7F00000000000000ULL)
#define SCALE_BITINDEX		(56ULL)
#define SIGN_MASK			(0x8000000000000000ULL)
#define SIGN_BITINDEX		(63ULL)
#define FRACTION_MASK		(0x00FFFFFFFFFFFFFFULL)
#define FRACTION_BITINDEX	(0L)

static inline int _decimal_is_overflow(const mg_uint256_t *value)
{
	return !mg_uint256_less_than(value, mg_uint256_get_10eN(DIGIT_MAX));
}

static inline unsigned long long _decimal_get_bits(unsigned long long dest, unsigned long long mask, int bitindex)
{
	return (dest & mask) >> bitindex;
}

static inline unsigned long long _deciaml_set_bits(unsigned long long dest, unsigned long long value, unsigned long long mask, int bitindex)
{
	return (dest & ~mask) | ((value << bitindex) & mask);
}

static inline int _decimal_get_sign(const mg_decimal_t *value)
{
	return (int)_decimal_get_bits(
		value->w[INFO_INDEX], SIGN_MASK, SIGN_BITINDEX);
}

static inline void _decimal_set_sign(mg_decimal_t *value, int sign)
{
	value->w[INFO_INDEX] = _deciaml_set_bits(
		value->w[INFO_INDEX],
		sign,
		SIGN_MASK,
		SIGN_BITINDEX);
}

static inline int _decimal_get_scale(const mg_decimal_t *value)
{
	int bits = (int)_decimal_get_bits(
		value->w[INFO_INDEX], SCALE_MASK, SCALE_BITINDEX);
	return (bits & SCALE_SIGNEXPAND) == 0 ? bits: (short)(bits | SCALE_SIGNEXPAND);
}

static inline void _decimal_get_fraction(const mg_decimal_t *value, mg_uint256_t *buf)
{
	buf->word[0] = value->w[0];
	buf->word[1] = _decimal_get_bits(
		value->w[INFO_INDEX], FRACTION_MASK, FRACTION_BITINDEX);
	buf->word[2] = 0;
	buf->word[3] = 0;
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

	value->w[0] = fraction->word[0];
	value->w[INFO_INDEX] =
		(((unsigned long long)sign << SIGN_BITINDEX) & SIGN_MASK) | 
		(((unsigned long long)scale << SCALE_BITINDEX) & SCALE_MASK) |
		(((unsigned long long)fraction->word[1] << FRACTION_BITINDEX) & FRACTION_MASK);

	return 0;
_ERROR:
	return err;
}

static inline void _decimal_set2(mg_decimal_t *value, int sign, int scale, const mg_uint256_t *fraction)
{
	assert(_decimal_is_overflow(fraction) == 0);
	assert(SCALE_MIN <= scale && scale <= SCALE_MAX);
	assert(sign == 0 || sign == 1);

	value->w[0] = fraction->word[0];
	value->w[INFO_INDEX] =
		(((unsigned long long)sign << SIGN_BITINDEX) & SIGN_MASK) |
		(((unsigned long long)scale << SCALE_BITINDEX) & SCALE_MASK) |
		(((unsigned long long)fraction->word[1] << FRACTION_BITINDEX) & FRACTION_MASK);
}
