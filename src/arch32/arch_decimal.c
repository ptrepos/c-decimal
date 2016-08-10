/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2011/03/15 
 *
 * 128 bit decimal implements.
 */
#include <magica/decimal/decimal.h>

#include <arch_priv_decimal.h>
#include <arch_priv_uint256.h>

MG_DECIMAL_API void mg_decimal_set_binary(
		/*out*/mg_decimal *value, 
		uint64_t high, 
		uint64_t low)
{
	value->w[3] = (uint32_t)(high >> 32ULL);
	value->w[2] = (uint32_t)(high);
	value->w[1] = (uint32_t)(low >> 32ULL);
	value->w[0] = (uint32_t)(low);
}

MG_DECIMAL_API void mg_decimal_get_binary(
		const mg_decimal *value, 
		/*out*/uint64_t *high, 
		/*out*/uint64_t *low)
{
	*high = ((uint64_t)value->w[3] << 32ULL) | (uint64_t)value->w[2];
	*low = ((uint64_t)value->w[1] << 32ULL) | (uint64_t)value->w[0];
}
