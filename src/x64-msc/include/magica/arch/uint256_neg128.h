#pragma once

#include <intrin.h>
#include <magica/arch/def.h>

static inline void mg_uint256_neg128(mg_uint256_t *op1)
{
	unsigned char c;
	
	op1->word[0] = ~op1->word[0];
	op1->word[1] = ~op1->word[1];
	
	c = _addcarry_u64(0, op1->word[0], 1, &op1->word[0]);
	c = _addcarry_u64(c, op1->word[1], 0, &op1->word[1]);
}
