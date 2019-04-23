/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2011/03/15 
 * 
 * 128 bit decimal implements.
 */
#pragma once

#include<stdint.h>
#include<stdbool.h>

#include<magica/decimal/libmgdecimal.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mg_decimal
{
	union {
		struct {
			unsigned int word0;
			unsigned int word1;
			unsigned int word2;
			unsigned int word3;
		} b32;
		struct {
			unsigned long long word0;
			unsigned long long word1;
		} b64;
	} u;
} mg_decimal_t;

typedef struct _mg_error	*mg_error_t;

#define mgE_OVERFLOW			((mg_error_t)2) /* number type overflow. */
#define mgE_BUFFER_NOT_ENOUGH	((mg_error_t)5) /* buffer not enough */
#define mgE_CONVERT				((mg_error_t)6) /* convert error. */
#define mgE_ZERODIVIDE			((mg_error_t)7) /* zero divide error. */

#define mgDECIMAL_INITALIZER	{0,0}

MG_DECIMAL_API void mg_decimal_zero(/*out*/mg_decimal_t *value);
MG_DECIMAL_API void mg_decimal_one(/*out*/mg_decimal_t *value);
MG_DECIMAL_API void mg_decimal_minus_one(/*out*/mg_decimal_t *value);
MG_DECIMAL_API void mg_decimal_min_value(/*out*/mg_decimal_t *value);
MG_DECIMAL_API void mg_decimal_max_value(/*out*/mg_decimal_t *value);

MG_DECIMAL_API void mg_decimal_set_binary(/*out*/mg_decimal_t *value, unsigned long long high, unsigned long long low);
MG_DECIMAL_API void mg_decimal_get_binary(const mg_decimal_t *value, /*out*/unsigned long long *high, /*out*/unsigned long long *low);

MG_DECIMAL_API mg_error_t mg_decimal_value_of_int(int value, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_value_of_uint(unsigned int value, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_value_of_long_long(long long value, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_value_of_ulong_long(unsigned long long value, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_value_of_float(float value, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_value_of_double(double value, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_parse_string(const char *value, /*out*/mg_decimal_t *ret);

MG_DECIMAL_API mg_error_t mg_decimal_to_int(const mg_decimal_t *value, /*out*/int *ret);
MG_DECIMAL_API mg_error_t mg_decimal_to_uint(const mg_decimal_t *value, /*out*/unsigned int *ret);
MG_DECIMAL_API mg_error_t mg_decimal_to_long_long(const mg_decimal_t *value, /*out*/long long *ret);
MG_DECIMAL_API mg_error_t mg_decimal_to_ulong_long(const mg_decimal_t *value, /*out*/unsigned long long *ret);
MG_DECIMAL_API mg_error_t mg_decimal_to_float(const mg_decimal_t *value, /*out*/float *ret);
MG_DECIMAL_API mg_error_t mg_decimal_to_double(const mg_decimal_t *value, /*out*/double *ret);
MG_DECIMAL_API mg_error_t mg_decimal_to_string(const mg_decimal_t *value, /*out*/char *buffer, int buffer_size, /*out*/int *required_buffer_size);

MG_DECIMAL_API mg_error_t mg_decimal_negate(/*inout*/mg_decimal_t *op1);
MG_DECIMAL_API mg_error_t mg_decimal_add(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_subtract(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_multiply(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_divide(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_divide_and_modulus(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *quotient, /*out*/mg_decimal_t *reminder);
MG_DECIMAL_API mg_error_t mg_decimal_modulus(const mg_decimal_t *op1, const mg_decimal_t *op2, /*out*/mg_decimal_t *ret);

MG_DECIMAL_API bool mg_decimal_is_zero(const mg_decimal_t *op1);
MG_DECIMAL_API int mg_decimal_compare(const mg_decimal_t *op1, const mg_decimal_t *op2);

#define mgDECIMAL_ROUND_DOWN		(0)
#define mgDECIMAL_ROUND_UP			(1)
#define mgDECIMAL_ROUND_OFF			(2)
#define mgDECIMAL_CEILING			(3)
#define mgDECIMAL_FLOOR				(4)

MG_DECIMAL_API mg_error_t mg_decimal_round(/*inout*/mg_decimal_t *value, int precision, int type);

MG_DECIMAL_API mg_error_t mg_decimal_normalize(/*inout*/mg_decimal_t *value);
MG_DECIMAL_API mg_error_t mg_decimal_abs(const mg_decimal_t *value, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_min(const mg_decimal_t *value1, const mg_decimal_t *value2, /*out*/mg_decimal_t *ret);
MG_DECIMAL_API mg_error_t mg_decimal_max(const mg_decimal_t *value1, const mg_decimal_t *value2, /*out*/mg_decimal_t *ret);

#ifdef __cplusplus
}
#endif
