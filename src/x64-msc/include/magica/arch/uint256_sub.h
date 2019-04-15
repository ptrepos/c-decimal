#pragma once

#include <stdbool.h>
#include <intrin.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_sub(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char b;
	
	b = _subborrow_u64(0, op1->word[0], op2->word[0], &op1->word[0]);
	b = _subborrow_u64(b, op1->word[1], op2->word[1], &op1->word[1]);
	b = _subborrow_u64(b, op1->word[2], op2->word[2], &op1->word[2]);
	b = _subborrow_u64(b, op1->word[3], op2->word[3], &op1->word[3]);
	
	return (bool)b;
}
