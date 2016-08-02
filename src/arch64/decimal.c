/**
 * Copyright (C) dachicraft foundation 2015
 * @author t.hada 2011/03/15 
 *
 * 128 bit decimal implements.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <magica/decimal/decimal.h>

#include "uint256.h"

#define WORD_COUNT			(sizeof(mg_decimal) / sizeof(uint64_t))
#define INFO_INDEX			(WORD_COUNT-1)

#define SCALE_MASK			(0x7F00000000000000ULL)
#define SCALE_BITINDEX		(56ULL)
#define SIGN_MASK			(0x8000000000000000ULL)
#define SIGN_BITINDEX		(63ULL)
#define FRACTION_MASK		(0x00FFFFFFFFFFFFFFULL)
#define FRACTION_BITINDEX	(0L)

#define SIGN_POSITIVE		(0)
#define SIGN_NEGATIVE		(1)
#define SCALE_MIN			(-36)		// 最小小数点以下桁数
#define SCALE_MAX			(0)			// 最大小数点以下桁数
#define SCALE_SIGNEXPAND	(0xFFC0)	// 符号拡張ビット
#define DIGIT_MAX			(36)		// 有効桁数

#define ZERO_HIGH			(0x0000000000000000ULL)
#define ZERO_LOW			(0x0000000000000000ULL)
#define ONE_HIGH			(0x0000000000000000ULL)
#define ONE_LOW				(0x0000000000000001ULL)
#define MINUS_ONE_HIGH		(0x8000000000000000ULL)
#define MINUS_ONE_LOW		(0x0000000000000001ULL)
#define MAX_VALUE_HIGH		(0x00c097ce7bc90715ULL)
#define MAX_VALUE_LOW		(0xb34b9f0fffffffffULL)
#define MIN_VALUE_HIGH		(0x80c097ce7bc90715ULL)
#define MIN_VALUE_LOW		(0xb34b9f0fffffffffULL)

static mg_decimal_error __mg_decimal_round_down_max_digits(mg_uint256 *value, int scale, /*out*/int *rounded_scale);
static mg_decimal_error __mg_decimal_round_zero_digits(mg_uint256 *value, int scale, /*out*/int *rounded_scale);

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

static inline void put_c(char *buf, int bufSize, int index, char value)
{
	if (buf != NULL && index < bufSize)
		buf[index] = value;
}

MG_DECIMAL_API void mg_decimal_zero(/*out*/mg_decimal *value)
{
	mg_decimal_set_binary(value, ZERO_HIGH, ZERO_LOW);
}

MG_DECIMAL_API void mg_decimal_one(/*out*/mg_decimal *value)
{
	mg_decimal_set_binary(value, ONE_HIGH, ONE_LOW);
}

MG_DECIMAL_API void mg_decimal_minus_one(/*out*/mg_decimal *value)
{
	mg_decimal_set_binary(value, MINUS_ONE_HIGH, MINUS_ONE_LOW);
}

MG_DECIMAL_API void mg_decimal_min_value(/*out*/mg_decimal *value)
{
	mg_decimal_set_binary(value, MIN_VALUE_HIGH, MIN_VALUE_LOW);
}

MG_DECIMAL_API void mg_decimal_max_value(/*out*/mg_decimal *value)
{
	mg_decimal_set_binary(value, MAX_VALUE_HIGH, MAX_VALUE_LOW);
}

MG_DECIMAL_API void mg_decimal_set_binary(mg_decimal *value, uint64_t high, uint64_t low)
{
	value->w[1] = high;
	value->w[0] = low;
}

MG_DECIMAL_API void mg_decimal_get_binary(const mg_decimal *value, /*out*/uint64_t *high, /*out*/uint64_t *low)
{
	*high = value->w[1];
	*low = value->w[0];
}

MG_DECIMAL_API mg_decimal_error mg_decimal_value_of_int(int64_t value, /*out*/mg_decimal *dest)
{
	mg_decimal_error err;
	int sign;
	mg_uint256 fraction;

	if(value < 0) {
		sign = SIGN_NEGATIVE;
		value = -value;
	} else {
		sign = SIGN_POSITIVE;
	}
	mg_uint256_set(&fraction, value);

	err = __mg_set_decimal(dest, sign, 0, &fraction);
	assert(err == 0);

	return 0;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_to_int(const mg_decimal *value, /*out*/int64_t *dest)
{
	mg_decimal_error err;
	int sign;
	int scale;
	mg_uint256 buf1, buf2;
	mg_uint256 *fraction = &buf1, *tmp = &buf2;
	int64_t work;

	sign = __mg_decimal_get_sign(value);
	scale = __mg_decimal_get_scale(value);
	__mg_decimal_get_fraction(value, fraction);

	if(scale < 0) {
		err = mg_uint256_div(fraction, mg_uint256_get_10eN(-scale), tmp);
		if(err != 0)
			goto _ERROR;
		mg_uint256_swap(&fraction, &tmp);
	}

	// out of int64.
	if(mg_uint256_is_overflow_int64(fraction)) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}

	work = (int64_t)mg_uint256_get_int64(fraction);

	if(sign == SIGN_NEGATIVE) {
		work = -work;
	}

	*dest = work;

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_normalize(/*inout*/mg_decimal *value)
{
	mg_decimal_error err;
	int sign;
	int scale, rounded_scale;
	mg_uint256 buf1;
	mg_uint256 *fraction = &buf1;

	sign = __mg_decimal_get_sign(value);
	scale = __mg_decimal_get_scale(value);
	__mg_decimal_get_fraction(value, fraction);

	err = __mg_decimal_round_zero_digits(fraction, scale, /*out*/&rounded_scale);
	if (err != 0)
		goto _ERROR;
	scale = rounded_scale;

	if (mg_uint256_is_zero(fraction)) {
		sign = 0;
		scale = 0;
	}

	err = __mg_set_decimal(value, sign, scale, fraction);
	if(err != 0)
		goto _ERROR;

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_abs(const mg_decimal *value, /*out*/mg_decimal *ret)
{
	*ret = *value;

	__mg_decimal_set_sign(ret, 0);
	
	return 0;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_max(const mg_decimal *value1, const mg_decimal *value2, /*out*/mg_decimal *ret)
{
	if(mg_decimal_compare(value1, value2) < 0) {
		*ret = *value2;
	} else {
		*ret = *value1;
	}

	return 0;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_min(const mg_decimal *value1, const mg_decimal *value2, /*out*/mg_decimal *ret)
{
	if(mg_decimal_compare(value1, value2) < 0) {
		*ret = *value1;
	} else {
		*ret = *value2;
	}

	return 0;
}


MG_DECIMAL_API mg_decimal_error mg_decimal_parse_string(/*out*/mg_decimal *dest, const char *value)
{
	mg_decimal_error err;
	int sign;
	int scale, digits;
	mg_uint256 buf1, buf2;
	mg_uint256 *fraction = &buf1, *tmp = &buf2;
	int c;

	if(*value == 0) {
		err = MG_DECIMAL_ERROR_CONVERT;
		goto _ERROR;
	}
	switch(*value) {
	case '-':
		sign = SIGN_NEGATIVE;
		value++;
		break;
	case '+':
		value++;
	default:
		sign = SIGN_POSITIVE;
		break;
	}

	scale = 0;
	digits = 0;
	mg_uint256_set_zero(fraction);

	bool dot = false;

	if(*value == 0) {
		err = MG_DECIMAL_ERROR_CONVERT;
		goto _ERROR;
	}

	while(*value != 0) {
		c = *value;
		if(c == '0') {
			value++;
			// increment next
		} else if(('1' <= c && c <= '9') || c == '.') {
			break;
		} else  {
			err = MG_DECIMAL_ERROR_CONVERT;
			goto _ERROR;
		}
	}

	while(*value != 0) {
		c = *value;
		if('0' <= c && c <= '9') {
			if(digits < DIGIT_MAX) {
				// fraction * 10 + c
				mg_uint256_mul128(fraction, mg_uint256_get_10eN(1), tmp);
				mg_uint256_set(fraction, c - '0');
				mg_uint256_add(fraction, tmp);
			} else {
				scale++;
			}
			digits++;
			value++;
		} else if(c == '.') {
			dot = true;

			value++;
			if(*value == 0) {
				err = MG_DECIMAL_ERROR_CONVERT;
				goto _ERROR;
			}
			while(*value != 0) {
				int c = *value;
				if('0' <= c && c <= '9') {
					if(digits < DIGIT_MAX) {
						// fraction * 10 + c
						mg_uint256_mul128(fraction, mg_uint256_get_10eN(1), tmp);
						mg_uint256_set(fraction, c - '0');
						mg_uint256_add(fraction, tmp);

						digits++;
						scale--;
					}
					value++;
				} else {
					err = MG_DECIMAL_ERROR_CONVERT;
					goto _ERROR;
				}
			}
			break;
		}  else {
			err = MG_DECIMAL_ERROR_CONVERT;
			goto _ERROR;
		}
	}

	err = __mg_set_decimal(dest, sign, scale, fraction);
	if(err != 0)
		goto _ERROR;

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_to_string(const mg_decimal *value, /*out*/char *buf, int bufSize, /*out*/int *requireBufSize)
{
	mg_decimal_error err;
	int i, index;
	int sign;
	int scale;
	mg_uint256 buf1, buf2;
	mg_uint256 *fraction = &buf1, *tmp = &buf2;
	const mg_uint256 *v10e18;

	mg_decimal normalized_value = *value;
	err = mg_decimal_normalize(&normalized_value);
	if(err != 0) 
		goto _ERROR;

	v10e18 = mg_uint256_get_10eN(18);

	sign = __mg_decimal_get_sign(&normalized_value);
	scale = __mg_decimal_get_scale(&normalized_value);
	__mg_decimal_get_fraction(&normalized_value, fraction);

	index = 0;
	while(!mg_uint256_is_zero(fraction)) {
		uint64_t subfraction = 0;

		err = mg_uint256_div(fraction, v10e18, tmp);
		if(err != 0)
			return err;

		subfraction = mg_uint256_get_int64(fraction);

		if(mg_uint256_is_zero(tmp)) {
			while(subfraction > 0) {
				int c = subfraction % 10ULL;
				put_c(buf, bufSize, index++, (char) ('0' + c));
				if (++scale == 0)
					put_c(buf, bufSize, index++, '.');
				subfraction /= 10ULL;
			}
		} else {
			for (i = 0; i < 18; i++) {
				int c = subfraction % 10ULL;
				put_c(buf, bufSize, index++, (char) ('0' + c));
				if (++scale == 0)
					put_c(buf, bufSize, index++, '.');
				subfraction /= 10ULL;
			}
		}
		mg_uint256_swap(&fraction, &tmp);
	}
	while(scale < 0) {
		put_c(buf, bufSize, index++, '0');
		if (++scale == 0)
			put_c(buf, bufSize, index++, '.');
	}
	if(scale == 0)
		put_c(buf, bufSize, index++, '0');

	if(sign == SIGN_NEGATIVE)
		put_c(buf, bufSize, index++, '-');

	if(buf != NULL && index <= bufSize) {
		for(i = 0; i < index / 2; i++) {
			char c = buf[i];
			buf[i] = buf[index - i - 1];
			buf[index - i - 1] = c;
		}
	}
	put_c(buf, bufSize, index++, 0);

	*requireBufSize = index;

	if(bufSize < index) {
		err = MG_DECIMAL_ERROR_BUFFER_NOT_ENOUGH;
		goto _ERROR;
	}

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API bool mg_decimal_is_zero(const mg_decimal *op1)
{
	mg_uint256 fraction;

	__mg_decimal_get_fraction(op1, &fraction);

	return mg_uint256_is_zero(&fraction);
}

MG_DECIMAL_API int mg_decimal_compare(const mg_decimal *op1, const mg_decimal *op2)
{
	int sign1, sign2;
	int scale1, scale2;
	mg_uint256 buf1, buf2, buf3;
	mg_uint256 *fraction1 = &buf1, *fraction2 = &buf2, *tmp = &buf3;

	__mg_decimal_get_fraction(op1, fraction1);
	__mg_decimal_get_fraction(op2, fraction2);

	if (mg_uint256_is_zero(fraction1) && mg_uint256_is_zero(fraction2))
		return 0;

	sign1 = __mg_decimal_get_sign(op1);
	sign2 = __mg_decimal_get_sign(op2);

	if(sign1 != sign2) {
		if(mg_uint256_is_zero(fraction1))
			return sign2 == SIGN_POSITIVE ? -1: 1;
		else if (mg_uint256_is_zero(fraction2))
			return sign1 == SIGN_POSITIVE ? 1 : -1;
		else if(sign1 == SIGN_POSITIVE)
			return 1;
		else 
			return -1;
	}

	scale1 = __mg_decimal_get_scale(op1);
	scale2 = __mg_decimal_get_scale(op2);

	if(scale1 == scale2)
		return mg_uint256_compare(fraction1, fraction2);
	else if(scale1 < scale2) {
		mg_uint256_mul128(fraction2, mg_uint256_get_10eN(scale2 - scale1), tmp);
		return mg_uint256_compare(fraction1, tmp);
	} else {
		mg_uint256_mul128(fraction1, mg_uint256_get_10eN(scale1 - scale2), tmp);
		return mg_uint256_compare(tmp, fraction2);
	}
}

MG_DECIMAL_API mg_decimal_error mg_decimal_negate(mg_decimal *op1)
{
	if(mg_decimal_is_zero(op1))
		return 0;

	__mg_decimal_set_sign(op1, 1 - __mg_decimal_get_sign(op1));

	return 0;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_add(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;
	int borrow;
	int sign, sign1, sign2;
	int scale, scaleDiff, scale1, scale2;
	mg_uint256 buf1, buf2, buf3;
	mg_uint256 *fraction1 = &buf1, *fraction2 = &buf2, *tmp = &buf3;

	assert(ret != NULL);

	sign1 = __mg_decimal_get_sign(op1);
	scale1 = __mg_decimal_get_scale(op1);
	__mg_decimal_get_fraction(op1, fraction1);

	sign2 = __mg_decimal_get_sign(op2);
	scale2 = __mg_decimal_get_scale(op2);
	__mg_decimal_get_fraction(op2, fraction2);

	scaleDiff = scale1 - scale2;
	if (scaleDiff == 0) {
		scale = scale1;

		if (sign1 == sign2) {
			mg_uint256_add128(fraction1, fraction2);
			sign = sign1;
		} else {
			mg_uint256_sub128(fraction1, fraction2, &borrow);
			if (borrow == 0)
				sign = sign1;
			else {
				mg_uint256_neg128(fraction1);
				sign = sign1 == SIGN_POSITIVE ? SIGN_NEGATIVE : SIGN_POSITIVE;
			}
		}
	} else {
		if (scaleDiff > 0) {
			scale = scale2;

			mg_uint256_mul128(fraction1, mg_uint256_get_10eN(scaleDiff), tmp);
			mg_uint256_swap(&fraction1, &tmp);
		} else {
			scale = scale1;

			mg_uint256_mul128(fraction2, mg_uint256_get_10eN(-scaleDiff), tmp);
			mg_uint256_swap(&fraction2, &tmp);
		}

		if (sign1 == sign2) {
			mg_uint256_add(fraction1, fraction2);
			sign = sign1;
		} else {
			mg_uint256_sub(fraction1, fraction2, &borrow);
			if (borrow == 0)
				sign = sign1;
			else {
				mg_uint256_neg(fraction1);
				sign = sign1 == SIGN_POSITIVE ? SIGN_NEGATIVE : SIGN_POSITIVE;
			}
		}
	}

	if(__mg_decimal_is_overflow(fraction1)) {
		int rounded_scale;
		err = __mg_decimal_round_down_max_digits(fraction1, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;

		if(__mg_decimal_is_overflow(fraction1)) {
			err = MG_DECIMAL_ERROR_OVERFLOW;
			goto _ERROR;
		}
	}

	__mg_set_decimal2(ret, sign, scale, fraction1);

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_subtract(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;

	assert(ret != NULL);

	mg_decimal nagated_op2 = *op2;
	err = mg_decimal_negate(/*inout*/&nagated_op2);
	if(err != 0)
		goto _ERROR;
	err = mg_decimal_add(op1, &nagated_op2, ret);
	if(err != 0)
		goto _ERROR;

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_multiply(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;
	int sign, sign1, sign2;
	int scale, scale1, scale2;
	mg_uint256 buf1, buf2, buf3;
	mg_uint256 *fraction1 = &buf1, *fraction2 = &buf2, *fraction = &buf3;

	assert(ret != NULL);

	sign1 = __mg_decimal_get_sign(op1);
	scale1 = __mg_decimal_get_scale(op1);
	__mg_decimal_get_fraction(op1, fraction1);

	sign2 = __mg_decimal_get_sign(op2);
	scale2 = __mg_decimal_get_scale(op2);
	__mg_decimal_get_fraction(op2, fraction2);

	if (sign1 != sign2)
		sign = SIGN_NEGATIVE;
	else
		sign = SIGN_POSITIVE;

	scale = scale1 + scale2;

	mg_uint256_mul128(fraction1, fraction2, fraction);

	if (__mg_decimal_is_overflow(fraction)) {
		int rounded_scale;
		err = __mg_decimal_round_down_max_digits(fraction, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;

		if (__mg_decimal_is_overflow(fraction)) {
			err = MG_DECIMAL_ERROR_OVERFLOW;
			goto _ERROR;
		}
	}

	__mg_set_decimal2(ret, sign, scale, fraction);

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_divide(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;
	int overflow;
	int baseDigits;
	int sign, sign1, sign2;
	int scale, scale1, scale2;
	mg_uint256 buf1, buf2, buf3, buf4;
	mg_uint256 *fraction1 = &buf1, *fraction2 = &buf2, *q = &buf3, *tmp = &buf4;
	const mg_uint256 *v10e18;

	assert(ret != NULL);

	sign1 = __mg_decimal_get_sign(op1);
	scale1 = __mg_decimal_get_scale(op1);
	__mg_decimal_get_fraction(op1, fraction1);

	sign2 = __mg_decimal_get_sign(op2);
	scale2 = __mg_decimal_get_scale(op2);
	__mg_decimal_get_fraction(op2, fraction2);

	if (mg_uint256_is_zero(fraction2)) {
		err = MG_DECIMAL_ERROR_ZERODIVIDE;
		goto _ERROR;
	}

	if (sign1 != sign2)
		sign = SIGN_NEGATIVE;
	else
		sign = SIGN_POSITIVE;

	scale = scale1 - scale2;

	err = mg_uint256_div(fraction1, fraction2, /*out*/q);
	if(err != 0)
		goto _ERROR;

	v10e18 = mg_uint256_get_10eN(18);

	if(!mg_uint256_is_zero(fraction1)) {
		baseDigits = 0;
		while(!mg_uint256_is_zero(fraction1)) {
			int digits = mg_uint256_get_digits(q);
			if(digits > DIGIT_MAX)
				break;

			// fraction1 = fraction1 * 10^18
			mg_uint256_mul_with_words(fraction1, MG_UINT256_WORD_COUNT, v10e18, MG_UINT256_WORD_COUNT / 4, /*out*/tmp, /*out*/&overflow);
			assert(overflow == 0);
			mg_uint256_swap(&fraction1, &tmp);

			// q = q * 10^18
			mg_uint256_mul_with_words(q, MG_UINT256_WORD_COUNT, v10e18, MG_UINT256_WORD_COUNT / 4, /*out*/tmp, /*out*/&overflow);
			assert(overflow == 0);
			mg_uint256_swap(&q, &tmp);

			// q = q + fraction1 / fraction2;
			// fraction1 = fraction1 % fraction2;
			err = mg_uint256_div(/*inout*/fraction1, fraction2, /*out*/tmp);
			if(err != 0)
				goto _ERROR;
			mg_uint256_add(/*inout*/q, tmp);

			baseDigits += 18;
		}
		scale -= baseDigits;

		int rounded_scale;
		err = __mg_decimal_round_down_max_digits(q, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;

		err = __mg_decimal_round_zero_digits(q, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;
	}

	if(scale > 0) {
		mg_uint256_mul128(q, mg_uint256_get_10eN(scale), /*out*/tmp);
		scale = 0;
		if(__mg_decimal_is_overflow(tmp)) {
			err = MG_DECIMAL_ERROR_OVERFLOW;
			goto _ERROR;
		}
		__mg_set_decimal2(/*out*/ret, sign, scale, tmp);
	} else {
		__mg_set_decimal2(/*out*/ret, sign, scale, q);
	}


	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_divide_and_modulus(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *quotient, /*out*/mg_decimal *reminder)
{
	mg_decimal_error err;
	int sign, sign1, sign2;
	int scale, scale1, scale2;
	mg_uint256 buf1, buf2, buf3, buf4;
	mg_uint256 *fraction1 = &buf1, *fraction2 = &buf2, *q = &buf3, *tmp = &buf4;

	assert(quotient != NULL);
	assert(reminder != NULL);

	sign1 = __mg_decimal_get_sign(op1);
	scale1 = __mg_decimal_get_scale(op1);
	__mg_decimal_get_fraction(op1, fraction1);

	sign2 = __mg_decimal_get_sign(op2);
	scale2 = __mg_decimal_get_scale(op2);
	__mg_decimal_get_fraction(op2, fraction2);

	if (mg_uint256_is_zero(fraction2)) {
		err = MG_DECIMAL_ERROR_ZERODIVIDE;
		goto _ERROR;
	}

	if (sign1 != sign2)
		sign = SIGN_NEGATIVE;
	else
		sign = SIGN_POSITIVE;

	if(scale2 < scale1) {
		mg_uint256_mul128(fraction1, mg_uint256_get_10eN(scale1 - scale2), tmp);
		mg_uint256_swap(&fraction1, &tmp);

		scale1 = scale2;
	}
	scale = scale1 - scale2;

	err = mg_uint256_div(fraction1, fraction2, q);
	if (err != 0)
		goto _ERROR;

	if(scale < 0) {
		err = mg_uint256_div(q, mg_uint256_get_10eN(-scale), tmp);
		if (err != 0)
			goto _ERROR;
		mg_uint256_swap(&q, &tmp);
	}

	if(__mg_decimal_is_overflow(q)) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}

	__mg_set_decimal2(quotient, sign, 0, q);

	mg_decimal s;
	err = mg_decimal_multiply(quotient, op2, &s);
	if (err != 0)
		goto _ERROR;
	err = mg_decimal_subtract(op1, &s, reminder);
	if (err != 0)
		goto _ERROR;

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_modulus(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret)
{
	mg_decimal q;

	return mg_decimal_divide_and_modulus(op1, op2, &q, ret);
}

MG_DECIMAL_API mg_decimal_error mg_decimal_round(/*inout*/mg_decimal *value, int precision, int type)
{
	mg_decimal_error err;
	int sign;
	int scale, scaleDiff;
	mg_uint256 buf1, buf2, buf3;
	mg_uint256 *fraction = &buf1, *tmp = &buf2, *tmp2 = &buf3;
	uint64_t case_value;

	assert(value != NULL);

	scale = __mg_decimal_get_scale(value);
	if (-scale <= precision)
		return 0;

	sign = __mg_decimal_get_sign(value);
	__mg_decimal_get_fraction(value, fraction);

	scaleDiff = -scale - precision;

	err = mg_uint256_div(fraction, mg_uint256_get_10eN(scaleDiff), tmp);
	if(err != 0)
		goto _ERROR;

	if(mg_uint256_is_zero(fraction))
		return 0;
	switch(type)
	{
	case MG_DECIMAL_ROUND_DOWN:
		mg_uint256_swap(&fraction, &tmp);
		break;
	case MG_DECIMAL_ROUND_UP:
		mg_uint256_set(fraction, 1);
		mg_uint256_add(fraction, tmp);
		break;
	case MG_DECIMAL_ROUND_OFF:
		err = mg_uint256_div(fraction, mg_uint256_get_10eN(scaleDiff - 1), tmp2);
		if (err != 0)
			goto _ERROR;
		case_value = mg_uint256_get_int64(tmp2);

		if(case_value >= 5) {
			mg_uint256_set(fraction, 1);
			mg_uint256_add(fraction, tmp);
		} else {
			mg_uint256_swap(&fraction, &tmp);
		}
	default:
		assert(type <= MG_DECIMAL_ROUND_OFF);
	}

	__mg_set_decimal2(value, sign, -precision, fraction);

	return 0;
_ERROR:
	return err;
}

static mg_decimal_error cutoff_invalid_digits(mg_uint256 *value, int scale, int digits, int *cuttedDigits)
{
	mg_decimal_error err;
	mg_uint256 buf1, buf2;
	mg_uint256  *work = &buf1, *tmp = &buf2;
	int vShift;
	int cutted;

	assert(value != NULL);
	assert(cuttedDigits != NULL);

	cutted = 0;

	*work = *value;

	//@J 現在の有効桁数より小数点の位置が大きければ
	//@J 小数点位置をベースにカット計算をする。
	if(digits < -scale)
		digits = -scale;
	if (digits > DIGIT_MAX) {
		vShift = digits - DIGIT_MAX;
		if(vShift >= -scale)
			return MG_DECIMAL_ERROR_OVERFLOW;

		err = mg_uint256_div(work, mg_uint256_get_10eN(vShift), tmp);
		if (err != 0)
			return err;
		cutted = -vShift;
		mg_uint256_swap(&work, &tmp);
		*value = *work;
	}

	*cuttedDigits = cutted;

	return 0;
}

static mg_decimal_error __mg_decimal_round_down_max_digits(mg_uint256 *value, int scale, int *rounded_scale)
{
	mg_decimal_error err;
	int digits;
	int rounded_digits;

	assert(value != NULL);

	digits = mg_uint256_get_digits(value);

	err = cutoff_invalid_digits(value, scale, digits, /*out*/&rounded_digits);
	if (err != 0)
		return err;
	*rounded_scale = scale - rounded_digits;

	return 0;
}

static int get_roundable_zero_digits(uint64_t value, uint64_t *cutted)
{
	int shiftDigits;

	shiftDigits = 0;

	if (value % 100000000L == 0) {
		shiftDigits += 8;
		value /= 100000000L;

		if (value % 100000000L == 0) {
			shiftDigits += 8;
			value /= 100000000L;
		}
	}
	if (value % 10000L == 0) {
		shiftDigits += 4;
		value /= 10000L;
	}
	if (value % 100L == 0) {
		shiftDigits += 2;
		value /= 100L;
	}
	if (value % 10L == 0) {
		shiftDigits += 1;
		value /= 10L;
	}

	*cutted = value;

	return shiftDigits;
}

static mg_decimal_error __mg_decimal_round_zero_digits(mg_uint256 *value, int scale, /*out*/int *cutted_scale)
{
	mg_decimal_error err;
	mg_uint256 buf1, buf2;
	mg_uint256  *work = &buf1, *tmp = &buf2;
	const mg_uint256 *shift;
	uint64_t v;
	int vShift;
	int cutted;

	assert(value != NULL);

	*work = *value;

	cutted = 0;

	// 小数点以下の下位0桁削除
	while (scale < 0) {
		int nShift = 18;
		if (-scale < nShift)
			nShift = -scale;
		err = mg_uint256_div(work, mg_uint256_get_10eN(nShift), tmp);
		if (err)
			goto _ERROR;

		if (mg_uint256_is_zero(work)) {
			mg_uint256_swap(&work, &tmp);
			scale += nShift;
			cutted += nShift;
		} else {
			v = mg_uint256_get_int64(work);
			vShift = get_roundable_zero_digits(v, &v);
			if (vShift <= 0)
				break;

			shift = mg_uint256_get_10eN(nShift - vShift);

			mg_uint256_mul128(tmp, shift, work);

			mg_uint256_set(tmp, v);
			mg_uint256_add(work, tmp);

			scale += vShift;
			cutted += vShift;

			break;
		}
	}

	if(cutted > 0)
		*value = *work;

	*cutted_scale = scale;

	return 0;
_ERROR:
	return err;
}

