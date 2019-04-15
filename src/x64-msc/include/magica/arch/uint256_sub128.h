#pragma once

#include <stdbool.h>
#include <intrin.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_sub128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char b;
	
	b = _subborrow_u64(0, op1->word[0], op2->word[0], &op1->word[0]);
	b = _subborrow_u64(b, op1->word[1], op2->word[1], &op1->word[1]);
	
	return (bool)b;
}
