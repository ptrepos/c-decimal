/**
 * Copyright (C) Takuo Hada 2015-2019
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

#include <decimal_impl.h>
#include <magica/arch/decimal_impl.h>
#include <magica/arch/uint256.h>
#include <double_impl.h>

static mg_error_t _truncate_max_digits(
		mg_uint256_t *value, 
		int scale, 
		/*out*/int *rounded_scale);
		
static mg_error_t _truncate_zero_digits(
		mg_uint256_t *value, 
		int scale, 
		/*out*/int *rounded_scale);

static mg_error_t _divide_internal(
		const mg_uint256_t *_fraction1, 
		const mg_uint256_t *_fraction2, 
		/*inout*/int *_scale,
		/*inout*/mg_uint256_t *_q);

#if defined(_M_X64) || defined(_M_ARM64) || defined(__amd64__) || defined(__arm64__)

MG_DECIMAL_API void mg_decimal_set_binary(
	mg_decimal_t *value,
	unsigned long long high,
	unsigned long long low)
{
	value->w[1] = high;
	value->w[0] = low;
}

MG_DECIMAL_API void mg_decimal_get_binary(
	const mg_decimal_t *value,
	/*out*/unsigned long long *high,
	/*out*/unsigned long long *low)
{
	*high = value->w[1];
	*low = value->w[0];
}

#else

MG_DECIMAL_API void mg_decimal_set_binary(
	/*out*/mg_decimal_t *value,
	unsigned long long high,
	unsigned long long low)
{
	value->w[3] = (uint32_t)(high >> 32ULL);
	value->w[2] = (uint32_t)(high);
	value->w[1] = (uint32_t)(low >> 32ULL);
	value->w[0] = (uint32_t)(low);
}

MG_DECIMAL_API void mg_decimal_get_binary(
	const mg_decimal_t *value,
	/*out*/unsigned long long *high,
	/*out*/unsigned long long *low)
{
	*high = ((unsigned long long)value->w[3] << 32ULL) | (unsigned long long)value->w[2];
	*low = ((unsigned long long)value->w[1] << 32ULL) | (unsigned long long)value->w[0];
}

#endif

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

MG_DECIMAL_API void mg_decimal_zero(/*out*/mg_decimal_t *value)
{
	mg_decimal_set_binary(value, ZERO_HIGH, ZERO_LOW);
}

MG_DECIMAL_API void mg_decimal_one(/*out*/mg_decimal_t *value)
{
	mg_decimal_set_binary(value, ONE_HIGH, ONE_LOW);
}

MG_DECIMAL_API void mg_decimal_minus_one(/*out*/mg_decimal_t *value)
{
	mg_decimal_set_binary(value, MINUS_ONE_HIGH, MINUS_ONE_LOW);
}

MG_DECIMAL_API void mg_decimal_min_value(/*out*/mg_decimal_t *value)
{
	mg_decimal_set_binary(value, MIN_VALUE_HIGH, MIN_VALUE_LOW);
}

MG_DECIMAL_API void mg_decimal_max_value(/*out*/mg_decimal_t *value)
{
	mg_decimal_set_binary(value, MAX_VALUE_HIGH, MAX_VALUE_LOW);
}

MG_DECIMAL_API mg_error_t mg_decimal_value_of_int(int value, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;
	int sign;
	mg_uint256_t fraction;

	if(value < 0) {
		sign = SIGN_NEGATIVE;
		mg_uint256_set(&fraction, ((uint32_t)-(value + 1)) + 1);
	} else {
		sign = SIGN_POSITIVE;
		mg_uint256_set(&fraction, value);
	}

	err = _decimal_set(ret, sign, 0, &fraction);
	assert(err == 0);

	return 0;
}

MG_DECIMAL_API mg_error_t mg_decimal_value_of_uint(unsigned int value, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;
	mg_uint256_t fraction;

	mg_uint256_set(&fraction, value);

	err = _decimal_set(ret, SIGN_POSITIVE, 0, &fraction);
	assert(err == 0);

	return 0;
}

MG_DECIMAL_API mg_error_t mg_decimal_value_of_long_long(long long value, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;
	int sign;
	mg_uint256_t fraction;

	if(value < 0) {
		sign = SIGN_NEGATIVE;
		mg_uint256_set(&fraction, ((unsigned long long)-(value + 1)) + 1);
	} else {
		sign = SIGN_POSITIVE;
		mg_uint256_set(&fraction, value);
	}

	err = _decimal_set(ret, sign, 0, &fraction);
	assert(err == 0);

	return 0;
}

MG_DECIMAL_API mg_error_t mg_decimal_value_of_ulong_long(unsigned long long value, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;
	mg_uint256_t fraction;

	mg_uint256_set(&fraction, value);

	err = _decimal_set(ret, SIGN_POSITIVE, 0, &fraction);
	assert(err == 0);

	return 0;
}

MG_DECIMAL_API mg_error_t mg_decimal_value_of_float(float value, /*out*/mg_decimal_t *ret)
{
	return mg_decimal_value_of_double((double)value, ret);
}

static unsigned int _pow2_int(int exponent)
{
	if(exponent <= 0) {
		return 1;
	} else if(exponent <= 1) {
		return 2;
	} else {
		int a = exponent / 2;
		int b = exponent % 2;
		
		int c = _pow2_int(a);
		
		if(b == 0)
			return c * c;
		else
			return c * c * 2;
	}
}

static void _pow2(int exponent, /*out*/mg_uint256_t *ret)
{
	if(exponent < 32) {
		mg_uint256_set(ret, _pow2_int(exponent));
		return;
	} else {
		int a = exponent / 2;
		int b = exponent % 2;
		
		mg_uint256_t c;
		_pow2(a, /*out*/&c);
		
		int overflow;
		mg_uint256_t tmp = c;
		overflow = mg_uint256_mul(&tmp, &c);
		
		if(b == 0) {
			*ret = tmp;
		} else {
			mg_uint256_t n2;
			mg_uint256_set(&n2, 2);
			*ret = tmp;
			overflow = mg_uint256_mul(ret, &n2);
		}
	}
}

MG_DECIMAL_API mg_error_t mg_decimal_value_of_double(double value, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;
	int sign;
	int double_sign;
	int double_scale;
	unsigned long long double_fraction;
	int double_status;
	
	_double_parse(
		value, 
		&double_sign, 
		&double_scale, 
		&double_fraction, 
		&double_status);

	if (double_status == DOUBLE_STATUS_INFINITY) {
		// Inifinity
		err = mgE_OVERFLOW;
		goto _ERROR;
	} else if (double_status == DOUBLE_STATUS_NAN) {
		// NaN
		err = mgE_CONVERT;
		goto _ERROR;
	} else if(double_status == DOUBLE_STATUS_ZERO) {
		// ZERO
		mg_decimal_zero(/*out*/ret);
		goto _EXIT;
	} else if(double_status == DOUBLE_STATUS_UNNORMAL) {
		// UNNORMAL
		err = mgE_CONVERT;
		goto _ERROR;
	}
	
	if(double_sign == DOUBLE_SIGN_POSITIVE) {
		sign = SIGN_POSITIVE;
	} else {
		sign = SIGN_NEGATIVE;
	}
	
	double_scale -= DOUBLE_FRACTION_BITS;

	if (double_scale == 0) {
		mg_uint256_t fraction;
		mg_uint256_set(/*out*/&fraction, double_fraction);
		
		err = _decimal_set(/*out*/ret, sign, 0, &fraction);
		if(err != 0)
			goto _ERROR;
	} else if (double_scale > 0) {
		if(double_scale > 128) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
		mg_uint256_t fraction;
		mg_uint256_set(/*out*/&fraction, double_fraction);
		
		mg_uint256_left_shift(/*inout*/&fraction, double_scale);
		
		err = _decimal_set(/*out*/ret, sign, 0, &fraction);
		if(err != 0)
			goto _ERROR;
	} else if (double_scale < 0) {
		mg_uint256_t fraction;
		
		mg_uint256_set(/*out*/&fraction, double_fraction);
		mg_uint256_right_shift(/*inout*/&fraction, -double_scale);
		
		// 整数部取得
		mg_decimal_t integer_part;
		err = _decimal_set(/*out*/&integer_part, sign, 0, &fraction);
		if(err != 0)
			goto _ERROR;
		
		// 小数部取得～基数変換
		if(double_scale < -(128 + DOUBLE_FRACTION_BITS + 1)) {
			*ret = integer_part;
		} else {
			// decimal_part = decimal_part_bits / (radix ^ scale);
			// x = integer_part + decimal_part;
			mg_decimal_t decimal_part;
			mg_uint256_t decimal_part_bits;
			mg_uint256_t radix_conv;
			
			mg_uint256_set(/*out*/&decimal_part_bits, double_fraction);
			mg_uint256_get_bits(/*inout*/&decimal_part_bits, -double_scale);

			_pow2(-double_scale, /*out*/&radix_conv);

			int scale = 0;
			mg_uint256_t q = {0};

			err = _divide_internal(
					&decimal_part_bits, &radix_conv, 
					/*inout*/&scale, /*inout*/&q);
			if(err != 0)
				goto _ERROR;

			err = _decimal_set(/*out*/&decimal_part, sign, scale, &q);
			if(err != 0)
				goto _ERROR;

			err = mg_decimal_add(&integer_part, &decimal_part, /*out*/ret);
			if(err != 0)
				goto _ERROR;
		}
	}

_EXIT:
	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_to_int(const mg_decimal_t *value, /*out*/int *ret)
{
	mg_error_t err;
	int sign;
	int scale;
	mg_uint256_t buf1, buf2;
	mg_uint256_t *fraction = &buf1, *tmp = &buf2;
	int work;

	sign = _decimal_get_sign(value);
	scale = _decimal_get_scale(value);
	_decimal_get_fraction(value, fraction);

	if (scale < 0) {
		err = (mg_error_t)mg_uint256_div(fraction, mg_uint256_get_10eN(-scale), tmp);
		if (err != 0)
			goto _ERROR;
		mg_uint256_swap(&fraction, &tmp);
	}

	if (sign == SIGN_NEGATIVE) {
		mg_uint256_set(/*out*/tmp, (unsigned long long)INT32_MAX + 1);

		// out of int64.
		if (mg_uint256_compare(fraction, tmp) > 0) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	}
	else {
		mg_uint256_set(/*out*/tmp, INT32_MAX);

		// out of int64.
		if (mg_uint256_compare(fraction, tmp) > 0) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	}

	work = (int)mg_uint256_get_uint64(fraction);

	if (sign == SIGN_NEGATIVE) {
		work = -work;
	}

	*ret = work;

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_to_uint(const mg_decimal_t *value, /*out*/unsigned int *ret)
{
	mg_error_t err;
	int sign;
	int scale;
	mg_uint256_t buf1, buf2;
	mg_uint256_t *fraction = &buf1, *tmp = &buf2;

	sign = _decimal_get_sign(value);
	scale = _decimal_get_scale(value);
	_decimal_get_fraction(value, fraction);

	if (scale < 0) {
		err = (mg_error_t)mg_uint256_div(fraction, mg_uint256_get_10eN(-scale), tmp);
		if (err != 0)
			goto _ERROR;
		mg_uint256_swap(&fraction, &tmp);
	}

	if (sign == SIGN_NEGATIVE) {
		mg_uint256_set(/*out*/tmp, 0);

		if (mg_uint256_compare(fraction, tmp) > 0) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	}
	else {
		mg_uint256_set(/*out*/tmp, UINT32_MAX);

		// out of int64.
		if (mg_uint256_compare(fraction, tmp) > 0) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	}

	*ret = (unsigned int)mg_uint256_get_uint64(fraction);

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_to_long_long(const mg_decimal_t *value, /*out*/long long *ret)
{
	mg_error_t err;
	int sign;
	int scale;
	mg_uint256_t buf1, buf2;
	mg_uint256_t *fraction = &buf1, *tmp = &buf2;
	long long work;

	sign = _decimal_get_sign(value);
	scale = _decimal_get_scale(value);
	_decimal_get_fraction(value, fraction);

	if(scale < 0) {
		err = mg_uint256_div(fraction, mg_uint256_get_10eN(-scale), tmp);
		if(err != 0)
			goto _ERROR;
		mg_uint256_swap(&fraction, &tmp);
	}

	if (sign == SIGN_NEGATIVE) {
		mg_uint256_set(/*out*/tmp, (unsigned long long)INT64_MAX + 1);

		// out of int64.
		if (mg_uint256_compare(fraction, tmp) > 0) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	} else {
		mg_uint256_set(/*out*/tmp, INT64_MAX);

		// out of int64.
		if (mg_uint256_compare(fraction, tmp) > 0) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	}

	work = (long long)mg_uint256_get_uint64(fraction);

	if(sign == SIGN_NEGATIVE) {
		work = -work;
	}

	*ret = work;

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_to_ulong_long(const mg_decimal_t *value, /*out*/unsigned long long *ret)
{
	mg_error_t err;
	int sign;
	int scale;
	mg_uint256_t buf1, buf2;
	mg_uint256_t *fraction = &buf1, *tmp = &buf2;

	sign = _decimal_get_sign(value);
	scale = _decimal_get_scale(value);
	_decimal_get_fraction(value, fraction);

	if (scale < 0) {
		err = mg_uint256_div(fraction, mg_uint256_get_10eN(-scale), tmp);
		if (err != 0)
			goto _ERROR;
		mg_uint256_swap(&fraction, &tmp);
	}

	if (sign == SIGN_NEGATIVE) {
		mg_uint256_set(/*out*/tmp, 0);

		if (mg_uint256_compare(fraction, tmp) > 0) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	} else {
		mg_uint256_set(/*out*/tmp, UINT64_MAX);

		// out of int64.
		if (mg_uint256_compare(fraction, tmp) > 0) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	}

	*ret = mg_uint256_get_uint64(fraction);

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_to_float(const mg_decimal_t *value, /*out*/float *ret)
{
	mg_error_t err;
	double v;
	
	err = mg_decimal_to_double(value, /*out*/&v);
	if(err != 0)
		goto _ERROR;
	
	*ret = (float)v;
	
	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_to_double(const mg_decimal_t *value, /*out*/double *ret)
{
	mg_error_t err;
	int sign;
	int scale;
	mg_uint256_t fraction;
	
	sign = _decimal_get_sign(value);
	scale = _decimal_get_scale(value);
	_decimal_get_fraction(value, &fraction);

	if(mg_uint256_is_zero(&fraction)) {
		*ret = 0.0;
		goto _EXIT;
	}
	
	if(scale == 0) {
		// 整数値
		int double_sign;
		int double_scale;

		double_sign = sign;

		int index = mg_uint256_get_max_bit_index(&fraction);
		if(index > DOUBLE_FRACTION_BITS) {
			mg_uint256_right_shift(/*inout*/&fraction, index - DOUBLE_FRACTION_BITS);
		} else if(index < DOUBLE_FRACTION_BITS) {
			mg_uint256_left_shift(/*inout*/&fraction, DOUBLE_FRACTION_BITS - index);
		}
		double_scale = index;

		_double_set(/*out*/ret, 
			double_sign, 
			double_scale, 
			mg_uint256_get_uint64(&fraction));
	} else if(scale > 0) {
		// 整数値*10^scale
		assert(0);
	} else if(scale < 0) {
		// 整数値+小数値
		mg_uint256_t integer_part, decimal_part;
		int double_sign, double_scale;

		mg_uint256_div(&fraction, mg_uint256_get_10eN(-scale), &integer_part);

		double_sign = sign;

		int index = mg_uint256_get_max_bit_index(&integer_part);
		if(index < 0) {
			mg_uint256_left_shift(/*inout*/&fraction, 128);

			err = mg_uint256_div(&fraction, mg_uint256_get_10eN(-scale), &decimal_part);
			if(err != 0)
				goto _ERROR;

			index = mg_uint256_get_max_bit_index(&decimal_part);
			if(index < 0) {
				*ret = 0.0;
				goto _EXIT;
			} else if(index > DOUBLE_FRACTION_BITS) {
				mg_uint256_right_shift(/*inout*/&decimal_part, index - DOUBLE_FRACTION_BITS);
			} else  if(index < DOUBLE_FRACTION_BITS) {
				mg_uint256_left_shift(/*inout*/&decimal_part, DOUBLE_FRACTION_BITS - index);
			}
			double_scale = index - 128;

			fraction = decimal_part;
		} else if(index > DOUBLE_FRACTION_BITS + 1) {
			mg_uint256_right_shift(/*inout*/&integer_part, index - (DOUBLE_FRACTION_BITS + 1));
			double_scale = index;

			fraction = integer_part;
		} else if(index < DOUBLE_FRACTION_BITS + 1) {
			double_scale = index;

			mg_uint256_left_shift(/*inout*/&integer_part, DOUBLE_FRACTION_BITS - index);
			mg_uint256_left_shift(/*inout*/&fraction, DOUBLE_FRACTION_BITS - index);

			err = mg_uint256_div(&fraction, mg_uint256_get_10eN(-scale), &decimal_part);
			if(err != 0)
				goto _ERROR;

			mg_uint256_or(/*inout*/&integer_part, &decimal_part);

			fraction = integer_part;
		} else {
			double_scale = DOUBLE_FRACTION_BITS + 1;

			fraction = integer_part;
		}
		_double_set(/*out*/ret, 
			double_sign, 
			double_scale, 
			mg_uint256_get_uint64(&fraction));
	}

_EXIT:
	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_parse_string(const char *value, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;
	int sign;
	int scale, digits;
	mg_uint256_t buf1, buf2;
	mg_uint256_t *fraction = &buf1, *tmp = &buf2;
	int c;

	if(*value == 0) {
		err = mgE_CONVERT;
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
		err = mgE_CONVERT;
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
			err = mgE_CONVERT;
			goto _ERROR;
		}
	}

	while(*value != 0) {
		c = *value;
		if('0' <= c && c <= '9') {
			if(digits < DIGIT_MAX) {
				// fraction * 10 + c
				mg_uint256_mul128(fraction, mg_uint256_get_10eN(1));
				mg_uint256_set(tmp, c - '0');
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
				err = mgE_CONVERT;
				goto _ERROR;
			}
			while(*value != 0) {
				int c = *value;
				if('0' <= c && c <= '9') {
					if(digits < DIGIT_MAX) {
						// fraction * 10 + c
						mg_uint256_mul128(fraction, mg_uint256_get_10eN(1));
						mg_uint256_set(tmp, c - '0');
						mg_uint256_add(fraction, tmp);

						digits++;
						scale--;
					}
					value++;
				} else {
					err = mgE_CONVERT;
					goto _ERROR;
				}
			}
			break;
		}  else {
			err = mgE_CONVERT;
			goto _ERROR;
		}
	}

	err = _decimal_set(ret, sign, scale, fraction);
	if(err != 0)
		goto _ERROR;

	return 0;
_ERROR:
	return err;
}

static inline void _put_c(char *buf, int bufSize, int index, char value)
{
	if (buf != NULL && index < bufSize)
		buf[index] = value;
}

MG_DECIMAL_API mg_error_t mg_decimal_to_string(const mg_decimal_t *value, /*out*/char *buf, int bufSize, /*out*/int *requireBufSize)
{
	mg_error_t err;
	int i, index;
	int sign;
	int scale;
	mg_uint256_t buf1, buf2;
	mg_uint256_t *fraction = &buf1, *tmp = &buf2;
	const mg_uint256_t *v10e18;

	mg_decimal_t normalized_value = *value;
	err = mg_decimal_normalize(&normalized_value);
	if(err != 0) 
		goto _ERROR;

	v10e18 = mg_uint256_get_10eN(18);

	sign = _decimal_get_sign(&normalized_value);
	scale = _decimal_get_scale(&normalized_value);
	_decimal_get_fraction(&normalized_value, fraction);

	index = 0;
	while(!mg_uint256_is_zero(fraction)) {
		unsigned long long subfraction = 0;

		err = mg_uint256_div(fraction, v10e18, tmp);
		if(err != 0)
			return err;

		subfraction = mg_uint256_get_int64(fraction);

		if(mg_uint256_is_zero(tmp)) {
			while(subfraction > 0) {
				int c = subfraction % 10ULL;
				_put_c(buf, bufSize, index++, (char) ('0' + c));
				if (++scale == 0)
					_put_c(buf, bufSize, index++, '.');
				subfraction /= 10ULL;
			}
		} else {
			for (i = 0; i < 18; i++) {
				int c = subfraction % 10ULL;
				_put_c(buf, bufSize, index++, (char) ('0' + c));
				if (++scale == 0)
					_put_c(buf, bufSize, index++, '.');
				subfraction /= 10ULL;
			}
		}
		mg_uint256_swap(&fraction, &tmp);
	}
	while(scale < 0) {
		_put_c(buf, bufSize, index++, '0');
		if (++scale == 0)
			_put_c(buf, bufSize, index++, '.');
	}
	if(scale == 0)
		_put_c(buf, bufSize, index++, '0');

	if(sign == SIGN_NEGATIVE)
		_put_c(buf, bufSize, index++, '-');

	if(buf != NULL && index <= bufSize) {
		for(i = 0; i < index / 2; i++) {
			char c = buf[i];
			buf[i] = buf[index - i - 1];
			buf[index - i - 1] = c;
		}
	}
	_put_c(buf, bufSize, index++, 0);

	*requireBufSize = index;

	if(bufSize < index) {
		err = mgE_BUFFER_NOT_ENOUGH;
		goto _ERROR;
	}

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_normalize(/*inout*/mg_decimal_t *value)
{
	mg_error_t err;
	int sign;
	int scale, rounded_scale;
	mg_uint256_t buf1;
	mg_uint256_t *fraction = &buf1;

	sign = _decimal_get_sign(value);
	scale = _decimal_get_scale(value);
	_decimal_get_fraction(value, fraction);

	err = _truncate_zero_digits(fraction, scale, /*out*/&rounded_scale);
	if (err != 0)
		goto _ERROR;
	scale = rounded_scale;

	if (mg_uint256_is_zero(fraction)) {
		sign = 0;
		scale = 0;
	}

	err = _decimal_set(value, sign, scale, fraction);
	if(err != 0)
		goto _ERROR;

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_abs(const mg_decimal_t *value, /*out*/mg_decimal_t *ret)
{
	*ret = *value;

	_decimal_set_sign(ret, 0);
	
	return 0;
}

MG_DECIMAL_API mg_error_t mg_decimal_max(const mg_decimal_t *value1, const mg_decimal_t *value2, /*out*/mg_decimal_t *ret)
{
	if(mg_decimal_compare(value1, value2) < 0) {
		*ret = *value2;
	} else {
		*ret = *value1;
	}

	return 0;
}

MG_DECIMAL_API mg_error_t mg_decimal_min(const mg_decimal_t *value1, const mg_decimal_t *value2, /*out*/mg_decimal_t *ret)
{
	if(mg_decimal_compare(value1, value2) < 0) {
		*ret = *value1;
	} else {
		*ret = *value2;
	}

	return 0;
}

MG_DECIMAL_API bool mg_decimal_is_zero(const mg_decimal_t *op1)
{
	mg_uint256_t fraction;

	_decimal_get_fraction(op1, &fraction);

	return mg_uint256_is_zero(&fraction);
}

MG_DECIMAL_API int mg_decimal_compare(const mg_decimal_t *op1, const mg_decimal_t *op2)
{
	int sign1, sign2;
	int scale1, scale2;
	mg_uint256_t buf1, buf2, buf3;
	mg_uint256_t *fraction1 = &buf1, *fraction2 = &buf2, *tmp = &buf3;

	_decimal_get_fraction(op1, fraction1);
	_decimal_get_fraction(op2, fraction2);

	sign1 = _decimal_get_sign(op1);
	sign2 = _decimal_get_sign(op2);

	if(sign1 != sign2) {
		if(mg_uint256_is_zero128(fraction1))
			return sign2 == SIGN_POSITIVE ? -1: 1;
		else if (mg_uint256_is_zero128(fraction2))
			return sign1 == SIGN_POSITIVE ? 1 : -1;
		else if(sign1 == SIGN_POSITIVE)
			return 1;
		else 
			return -1;
	}

	scale1 = _decimal_get_scale(op1);
	scale2 = _decimal_get_scale(op2);

	int flag = 1;
	if(sign1)
		flag = -1;

	if(scale1 == scale2)
		return mg_uint256_compare(fraction1, fraction2) * flag;
	else if(scale1 < scale2) {
		mg_uint256_mul128(fraction2, mg_uint256_get_10eN(scale2 - scale1));
		return mg_uint256_compare(fraction1, fraction2) * flag;
	} else {
		mg_uint256_mul128(fraction1, mg_uint256_get_10eN(scale1 - scale2));
		return mg_uint256_compare(fraction1, fraction2) * flag;
	}
}

MG_DECIMAL_API mg_error_t mg_decimal_negate(mg_decimal_t *op1)
{
	if(mg_decimal_is_zero(op1))
		return 0;

	_decimal_set_sign(op1, 1 - _decimal_get_sign(op1));

	return 0;
}

MG_DECIMAL_API mg_error_t mg_decimal_add(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;
	int borrow;
	int sign, sign1, sign2;
	int scale, scaleDiff, scale1, scale2;
	mg_uint256_t buf1, buf2, buf3;
	mg_uint256_t *fraction1 = &buf1, *fraction2 = &buf2, *tmp = &buf3;

	assert(ret != NULL);

	sign1 = _decimal_get_sign(op1);
	scale1 = _decimal_get_scale(op1);
	_decimal_get_fraction(op1, fraction1);

	sign2 = _decimal_get_sign(op2);
	scale2 = _decimal_get_scale(op2);
	_decimal_get_fraction(op2, fraction2);

	scaleDiff = scale1 - scale2;
	if (scaleDiff == 0) {
		scale = scale1;

		if (sign1 == sign2) {
			mg_uint256_add128(fraction1, fraction2);
			sign = sign1;
		} else {
			borrow = mg_uint256_sub128(fraction1, fraction2);
			if (borrow == 0) {
				sign = sign1;
			} else {
				mg_uint256_neg128(fraction1);
				sign = sign1 == SIGN_POSITIVE ? SIGN_NEGATIVE : SIGN_POSITIVE;
			}
		}
	} else {
		if (scaleDiff == 0) {
			scale = scale1;
		} else if (scaleDiff > 0) {
			scale = scale2;
			mg_uint256_mul128(fraction1, mg_uint256_get_10eN(scaleDiff));
		} else {
			scale = scale1;
			mg_uint256_mul128(fraction2, mg_uint256_get_10eN(-scaleDiff));
		}

		if (sign1 == sign2) {
			mg_uint256_add(fraction1, fraction2);
			sign = sign1;
		} else {
			borrow = mg_uint256_sub(fraction1, fraction2);
			if (borrow == 0)
				sign = sign1;
			else {
				mg_uint256_neg(fraction1);
				sign = sign1 == SIGN_POSITIVE ? SIGN_NEGATIVE : SIGN_POSITIVE;
			}
		}
	}

	if(_decimal_is_overflow(fraction1)) {
		int rounded_scale;
		err = _truncate_max_digits(fraction1, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;

		if(_decimal_is_overflow(fraction1)) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	}

	_decimal_set2(ret, sign, scale, fraction1);

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_subtract(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;

	assert(ret != NULL);

	mg_decimal_t nagated_op2 = *op2;
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

MG_DECIMAL_API mg_error_t mg_decimal_multiply(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;
	int sign, sign1, sign2;
	int scale, scale1, scale2;
	mg_uint256_t buf1, buf2, buf3;
	mg_uint256_t *fraction1 = &buf1, *fraction2 = &buf2, *fraction = &buf3;

	assert(ret != NULL);

	sign1 = _decimal_get_sign(op1);
	scale1 = _decimal_get_scale(op1);
	_decimal_get_fraction(op1, fraction1);

	sign2 = _decimal_get_sign(op2);
	scale2 = _decimal_get_scale(op2);
	_decimal_get_fraction(op2, fraction2);

	if (sign1 != sign2)
		sign = SIGN_NEGATIVE;
	else
		sign = SIGN_POSITIVE;

	scale = scale1 + scale2;

	mg_uint256_mul128(fraction1, fraction2);

	if (_decimal_is_overflow(fraction1)) {
		int rounded_scale;
		err = _truncate_max_digits(fraction1, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;

		if (_decimal_is_overflow(fraction1)) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	}

	_decimal_set2(ret, sign, scale, fraction1);

	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_divide(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret)
{
	mg_error_t err;
	int sign, sign1, sign2;
	int scale, scale1, scale2;
	mg_uint256_t buf1, buf2, buf3, buf4;
	mg_uint256_t *fraction1 = &buf1, *fraction2 = &buf2, *q = &buf3, *tmp = &buf4;

	assert(ret != NULL);

	sign1 = _decimal_get_sign(op1);
	scale1 = _decimal_get_scale(op1);
	_decimal_get_fraction(op1, fraction1);

	sign2 = _decimal_get_sign(op2);
	scale2 = _decimal_get_scale(op2);
	_decimal_get_fraction(op2, fraction2);

	if (mg_uint256_is_zero(fraction2)) {
		err = mgE_ZERODIVIDE;
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

	err = _divide_internal(fraction1, fraction2, /*inout*/&scale, /*inout*/q);
	if(err != 0)
		goto _ERROR;

	if(scale > 0) {
		mg_uint256_mul128(q, mg_uint256_get_10eN(scale));
		scale = 0;
		if(_decimal_is_overflow(q)) {
			err = mgE_OVERFLOW;
			goto _ERROR;
		}
	}

	_decimal_set2(/*out*/ret, sign, scale, q);

	return 0;
_ERROR:
	return err;
}

static mg_error_t _divide_internal(
		const mg_uint256_t *_fraction1, 
		const mg_uint256_t *_fraction2, 
		/*inout*/int *_scale,
		/*inout*/mg_uint256_t *_q)
{
	mg_error_t err;
	int overflow;
	mg_uint256_t buf1, buf2, buf3, buf4;
	mg_uint256_t *fraction1 = &buf1;
	mg_uint256_t *fraction2 = &buf2;
	mg_uint256_t *quotient = &buf3;
	mg_uint256_t *tmp = &buf4;
	
	int scale = *_scale;

	const mg_uint256_t *v10e18 = mg_uint256_get_10eN(18);
	
	*fraction1 = *_fraction1;
	*fraction2 = *_fraction2;

	*quotient = *_q;

	if(!mg_uint256_is_zero(fraction1)) {
		int baseDigits = 0;
		do {
			if(scale - baseDigits < SCALE_MIN)
				break;
			if(mg_uint256_is_overflow128(quotient)) {
				break;
			}

			// fraction1 = fraction1 * 10^18
			overflow = mg_uint256_mul256x64(
					fraction1, v10e18);
			assert(overflow == 0);

			// q = q * 10^18
			overflow = mg_uint256_mul256x64(
					quotient, v10e18);
			assert(overflow == 0);

			// q = q + fraction1 / fraction2;
			// fraction1 = fraction1 % fraction2;
			err = mg_uint256_div(/*inout*/fraction1, fraction2, /*out*/tmp);
			if(err != 0)
				goto _ERROR;
			mg_uint256_add(/*inout*/quotient, tmp);

			baseDigits += 18;
		} while(!mg_uint256_is_zero(fraction1));

		scale -= baseDigits;

		int rounded_scale;
		err = _truncate_max_digits(quotient, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;
	}
	
	*_scale = scale;
	*_q = *quotient;
	
	return 0;
_ERROR:
	return err;
}

MG_DECIMAL_API mg_error_t mg_decimal_divide_and_modulus(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *quotient, /*out*/mg_decimal_t *reminder)
{
	mg_error_t err;
	int sign, sign1, sign2;
	int scale, scale1, scale2;
	mg_uint256_t buf1, buf2, buf3, buf4;
	mg_uint256_t *fraction1 = &buf1, *fraction2 = &buf2, *q = &buf3, *tmp = &buf4;

	assert(quotient != NULL);
	assert(reminder != NULL);

	sign1 = _decimal_get_sign(op1);
	scale1 = _decimal_get_scale(op1);
	_decimal_get_fraction(op1, fraction1);

	sign2 = _decimal_get_sign(op2);
	scale2 = _decimal_get_scale(op2);
	_decimal_get_fraction(op2, fraction2);

	if (mg_uint256_is_zero(fraction2)) {
		err = mgE_ZERODIVIDE;
		goto _ERROR;
	}

	if (sign1 != sign2)
		sign = SIGN_NEGATIVE;
	else
		sign = SIGN_POSITIVE;

	if(scale2 < scale1) {
		mg_uint256_mul128(fraction1, mg_uint256_get_10eN(scale1 - scale2));

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

	if(_decimal_is_overflow(q)) {
		err = mgE_OVERFLOW;
		goto _ERROR;
	}

	_decimal_set2(quotient, sign, 0, q);

	mg_decimal_t s;
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

MG_DECIMAL_API mg_error_t mg_decimal_modulus(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret)
{
	mg_decimal_t q;

	return mg_decimal_divide_and_modulus(op1, op2, &q, ret);
}

MG_DECIMAL_API mg_error_t mg_decimal_round(/*inout*/mg_decimal_t *value, int precision, int type)
{
	mg_error_t err;
	int sign;
	int scale, scaleDiff;
	mg_uint256_t buf1, buf2, buf3;
	mg_uint256_t *fraction = &buf1, *tmp = &buf2, *tmp2 = &buf3;
	unsigned long long case_value;

	assert(value != NULL);

	scale = _decimal_get_scale(value);
	if (-scale <= precision)
		return 0;

	sign = _decimal_get_sign(value);
	_decimal_get_fraction(value, fraction);

	scaleDiff = -scale - precision;

	err = mg_uint256_div(fraction, mg_uint256_get_10eN(scaleDiff), tmp);
	if(err != 0)
		goto _ERROR;

	if(mg_uint256_is_zero(fraction))
		return 0;
	switch(type)
	{
	case mgDECIMAL_ROUND_DOWN:
		mg_uint256_swap(&fraction, &tmp);
		break;
	case mgDECIMAL_ROUND_UP:
		mg_uint256_set(fraction, 1);
		mg_uint256_add(fraction, tmp);
		break;
	case mgDECIMAL_ROUND_OFF:
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
		break;
	case mgDECIMAL_CEILING:
		if(sign == SIGN_POSITIVE) {
			mg_uint256_set(fraction, 1);
			mg_uint256_add(fraction, tmp);
		} else {
			mg_uint256_swap(&fraction, &tmp);
		}
		break;
	case mgDECIMAL_FLOOR:
		if(sign == SIGN_POSITIVE) {
			mg_uint256_swap(&fraction, &tmp);
		} else {
			mg_uint256_set(fraction, 1);
			mg_uint256_add(fraction, tmp);
		}
		break;
	default:
		assert(type <= mgDECIMAL_ROUND_OFF);
	}

	if(precision < 0) {
		mg_uint256_mul128(fraction, mg_uint256_get_10eN(-precision));
		precision = 0;
	}

	_decimal_set2(value, sign, -precision, fraction);

	return 0;
_ERROR:
	return err;
}

static mg_error_t cutoff_invalid_digits(mg_uint256_t *value, int scale, int digits, int *cuttedDigits)
{
	mg_error_t err;
	mg_uint256_t buf1, buf2;
	mg_uint256_t  *work = &buf1, *tmp = &buf2;
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
			return mgE_OVERFLOW;

		err = mg_uint256_div(work, mg_uint256_get_10eN(vShift), tmp);
		if (err != 0)
			return err;
		cutted = -vShift;
		*value = *tmp;
	}

	*cuttedDigits = cutted;

	return 0;
}

static mg_error_t _truncate_max_digits(mg_uint256_t *value, int scale, int *rounded_scale)
{
	mg_error_t err;
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

static int get_roundable_zero_digits(unsigned long long value, unsigned long long *cutted)
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

static mg_error_t _truncate_zero_digits(mg_uint256_t *value, int scale, /*out*/int *cutted_scale)
{
	mg_error_t err;
	mg_uint256_t buf1, buf2;
	mg_uint256_t  *work = &buf1, *tmp = &buf2;
	const mg_uint256_t *shift;
	unsigned long long v;
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

			mg_uint256_mul128(tmp, shift);

			mg_uint256_set(work, v);
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
