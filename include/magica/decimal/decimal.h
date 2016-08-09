/**
 * Copyright (C) dachicraft foundation 2015
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
#if defined(__MG_DECIMAL_ARCH32)
	uint32_t w[4];
#else
	uint64_t w[2];
#endif
} mg_decimal;

typedef struct _mg_decimal_error	*mg_decimal_error;

#define MG_DECIMAL_ERROR_BUFFER_NOT_ENOUGH		((mg_decimal_error)1)
#define MG_DECIMAL_ERROR_CONVERT				((mg_decimal_error)2)
#define MG_DECIMAL_ERROR_OVERFLOW				((mg_decimal_error)3)
#define MG_DECIMAL_ERROR_ZERODIVIDE				((mg_decimal_error)4)

#define MG_DECIMAL_INITALIZER	{0,0}

MG_DECIMAL_API void mg_decimal_zero(/*out*/mg_decimal *value);
MG_DECIMAL_API void mg_decimal_one(/*out*/mg_decimal *value);
MG_DECIMAL_API void mg_decimal_minus_one(/*out*/mg_decimal *value);
MG_DECIMAL_API void mg_decimal_min_value(/*out*/mg_decimal *value);
MG_DECIMAL_API void mg_decimal_max_value(/*out*/mg_decimal *value);

MG_DECIMAL_API void mg_decimal_set_binary(/*out*/mg_decimal *value, uint64_t high, uint64_t low);
MG_DECIMAL_API void mg_decimal_get_binary(const mg_decimal *value, /*out*/uint64_t *high, /*out*/uint64_t *low);

MG_DECIMAL_API mg_decimal_error mg_decimal_value_of_int(int value, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_value_of_uint(unsigned int value, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_value_of_int64(int64_t value, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_value_of_uint64(uint64_t value, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_value_of_float(float value, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_value_of_double(double value, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_parse_string(const char *value, /*out*/mg_decimal *ret);

MG_DECIMAL_API mg_decimal_error mg_decimal_to_int(const mg_decimal *value, /*out*/int *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_to_uint(const mg_decimal *value, /*out*/unsigned int *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_to_int64(const mg_decimal *value, /*out*/int64_t *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_to_uint64(const mg_decimal *value, /*out*/uint64_t *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_to_float(const mg_decimal *value, /*out*/float *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_to_double(const mg_decimal *value, /*out*/double *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_to_string(const mg_decimal *value, /*out*/char *buffer, int buffer_size, /*out*/int *required_buffer_size);

MG_DECIMAL_API mg_decimal_error mg_decimal_negate(/*inout*/mg_decimal *op1);
MG_DECIMAL_API mg_decimal_error mg_decimal_add(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_subtract(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_multiply(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_divide(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_divide_and_modulus(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *quotient, /*out*/mg_decimal *reminder);
MG_DECIMAL_API mg_decimal_error mg_decimal_modulus(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret);

MG_DECIMAL_API bool mg_decimal_is_zero(const mg_decimal *op1);
MG_DECIMAL_API int mg_decimal_compare(const mg_decimal *op1, const mg_decimal *op2);

#define MG_DECIMAL_ROUND_DOWN		(0)
#define MG_DECIMAL_ROUND_UP			(1)
#define MG_DECIMAL_ROUND_OFF		(2)

#define MG_DECIMAL_ROUND_CEILING	MG_DECIMAL_ROUND_UP
#define MG_DECIMAL_ROUND_FLOOR		MG_DECIMAL_ROUND_DOWN

MG_DECIMAL_API mg_decimal_error mg_decimal_round(/*inout*/mg_decimal *value, int precision, int type);

MG_DECIMAL_API mg_decimal_error mg_decimal_normalize(/*inout*/mg_decimal *value);
MG_DECIMAL_API mg_decimal_error mg_decimal_abs(const mg_decimal *value, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_min(const mg_decimal *value1, const mg_decimal *value2, /*out*/mg_decimal *ret);
MG_DECIMAL_API mg_decimal_error mg_decimal_max(const mg_decimal *value1, const mg_decimal *value2, /*out*/mg_decimal *ret);

#ifdef __cplusplus
}
#endif
