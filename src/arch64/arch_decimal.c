/**
 * Copyright (C) dachicraft foundation 2015
 * @author t.hada 2011/03/15 
 *
 * 128 bit decimal implements.
 */
#include <magica/decimal/decimal.h>

#include <arch_priv_decimal.h>
#include <arch_priv_uint256.h>

MG_DECIMAL_API void mg_decimal_set_binary(
		mg_decimal *value, 
		uint64_t high, 
		uint64_t low)
{
	value->w[1] = high;
	value->w[0] = low;
}

MG_DECIMAL_API void mg_decimal_get_binary(
		const mg_decimal *value, 
		/*out*/uint64_t *high, 
		/*out*/uint64_t *low)
{
	*high = value->w[1];
	*low = value->w[0];
}
