#pragma once

#include <magica/decimal/decimal.h>

#include <arch_priv_uint256.h>
#include <priv_decimal.h>

#define WORD_COUNT			(sizeof(mg_decimal) / sizeof(uint64_t))
#define INFO_INDEX			(WORD_COUNT-1)

#define SCALE_MASK			(0x7F00000000000000ULL)
#define SCALE_BITINDEX		(56ULL)
#define SIGN_MASK			(0x8000000000000000ULL)
#define SIGN_BITINDEX		(63ULL)
#define FRACTION_MASK		(0x00FFFFFFFFFFFFFFULL)
#define FRACTION_BITINDEX	(0L)

static inline int __mg_decimal_is_overflow(const mg_uint256 *value)
{
	return mg_uint256_compare(value, mg_uint256_get_10eN(DIGIT_MAX)) >= 0;
}

static inline uint64_t __mg_decimal_get_bits(uint64_t dest, uint64_t mask, int bitindex)
{
	return (dest & mask) >> bitindex;
}

static inline uint64_t __mg_deciaml_set_bits(uint64_t dest, uint64_t value, uint64_t mask, int bitindex)
{
	return (dest & ~mask) | ((value << bitindex) & mask);
}

static inline int __mg_decimal_get_sign(const mg_decimal *value)
{
	return (int)__mg_decimal_get_bits(
		value->w[INFO_INDEX], SIGN_MASK, SIGN_BITINDEX);
}

static inline void __mg_decimal_set_sign(mg_decimal *value, int sign)
{
	value->w[INFO_INDEX] = __mg_deciaml_set_bits(
		value->w[INFO_INDEX],
		sign,
		SIGN_MASK,
		SIGN_BITINDEX);
}

static inline int __mg_decimal_get_scale(const mg_decimal *value)
{
	int bits = (int)__mg_decimal_get_bits(
		value->w[INFO_INDEX], SCALE_MASK, SCALE_BITINDEX);
	return (bits & SCALE_SIGNEXPAND) == 0 ? bits: (short)(bits | SCALE_SIGNEXPAND);
}

static inline void __mg_decimal_get_fraction(const mg_decimal *value, mg_uint256 *buf)
{
	buf->word[0] = value->w[0];
	buf->word[1] = __mg_decimal_get_bits(
		value->w[INFO_INDEX], FRACTION_MASK, FRACTION_BITINDEX);
	buf->word[2] = 0;
	buf->word[3] = 0;
}

static inline mg_decimal_error __mg_set_decimal(mg_decimal *value, int sign, int scale, const mg_uint256 *fraction)
{
	mg_decimal_error err;

	assert(sign == 0 || sign == 1);

	if(!(SCALE_MIN <= scale && scale <= SCALE_MAX)) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}

	if(__mg_decimal_is_overflow(fraction)) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}

	value->w[0] = fraction->word[0];
	value->w[INFO_INDEX] =
		(((uint64_t)sign << SIGN_BITINDEX) & SIGN_MASK) | 
		(((uint64_t)scale << SCALE_BITINDEX) & SCALE_MASK) | 
		(((uint64_t)fraction->word[1] << FRACTION_BITINDEX) & FRACTION_MASK);

	return 0;
_ERROR:
	return err;
}

static inline void __mg_set_decimal2(mg_decimal *value, int sign, int scale, const mg_uint256 *fraction)
{
	assert(__mg_decimal_is_overflow(fraction) == 0);
	assert(SCALE_MIN <= scale && scale <= SCALE_MAX);
	assert(sign == 0 || sign == 1);

	value->w[0] = fraction->word[0];
	value->w[INFO_INDEX] =
		(((uint64_t)sign << SIGN_BITINDEX) & SIGN_MASK) | 
		(((uint64_t)scale << SCALE_BITINDEX) & SCALE_MASK) | 
		(((uint64_t)fraction->word[1] << FRACTION_BITINDEX) & FRACTION_MASK);
}
