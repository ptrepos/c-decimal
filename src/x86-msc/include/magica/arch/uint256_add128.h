#pragma once

#include <intrin.h>
#include <magica/arch/def.h>

static inline void mg_uint256_add128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char c;
	
	c = _addcarry_u32(0, op1->word[0], op2->word[0], &op1->word[0]);
	c = _addcarry_u32(c, op1->word[1], op2->word[1], &op1->word[1]);
	c = _addcarry_u32(c, op1->word[2], op2->word[2], &op1->word[2]);
	c = _addcarry_u32(c, op1->word[3], op2->word[3], &op1->word[3]);
}
