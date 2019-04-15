#pragma once

#include <stdbool.h>
#include <intrin.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_sub(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char b;
	
	b = _subborrow_u32(0, op1->word[0], op2->word[0], &op1->word[0]);
	b = _subborrow_u32(b, op1->word[1], op2->word[1], &op1->word[1]);
	b = _subborrow_u32(b, op1->word[2], op2->word[2], &op1->word[2]);
	b = _subborrow_u32(b, op1->word[3], op2->word[3], &op1->word[3]);
	b = _subborrow_u32(b, op1->word[4], op2->word[4], &op1->word[4]);
	b = _subborrow_u32(b, op1->word[5], op2->word[5], &op1->word[5]);
	b = _subborrow_u32(b, op1->word[6], op2->word[6], &op1->word[6]);
	b = _subborrow_u32(b, op1->word[7], op2->word[7], &op1->word[7]);
	
	return (bool)b;
}
