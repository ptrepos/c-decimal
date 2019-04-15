#pragma once

#include <intrin.h>
#include <magica/arch/def.h>

static inline void mg_uint256_add(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char c;
	
	c = _addcarry_u32(0, op1->word[0], op2->word[0], &op1->word[0]);
	c = _addcarry_u32(c, op1->word[1], op2->word[1], &op1->word[1]);
	c = _addcarry_u32(c, op1->word[2], op2->word[2], &op1->word[2]);
	c = _addcarry_u32(c, op1->word[3], op2->word[3], &op1->word[3]);
	c = _addcarry_u32(c, op1->word[4], op2->word[4], &op1->word[4]);
	c = _addcarry_u32(c, op1->word[5], op2->word[5], &op1->word[5]);
	c = _addcarry_u32(c, op1->word[6], op2->word[6], &op1->word[6]);
	c = _addcarry_u32(c, op1->word[7], op2->word[7], &op1->word[7]);
}
