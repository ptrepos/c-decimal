#pragma once

#include <intrin.h>
#include <magica/arch/def.h>

static inline void mg_uint256_neg128(mg_uint256_t *op1)
{
	unsigned char c;
	
	op1->word[0] = ~op1->word[0];
	op1->word[1] = ~op1->word[1];
	op1->word[2] = ~op1->word[2];
	op1->word[3] = ~op1->word[3];
	
	c = _addcarry_u32(0, op1->word[0], 1, &op1->word[0]);
	c = _addcarry_u32(c, op1->word[1], 0, &op1->word[1]);
	c = _addcarry_u32(c, op1->word[2], 0, &op1->word[2]);
	c = _addcarry_u32(c, op1->word[3], 0, &op1->word[3]);
}
