#pragma once

#include <stdbool.h>
#include <intrin.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_less_than(const mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char b;
	unsigned long long dummy;
	
	b = _subborrow_u64(0, op1->word[0], op2->word[0], &dummy);
	b = _subborrow_u64(b, op1->word[1], op2->word[1], &dummy);
	b = _subborrow_u64(b, op1->word[2], op2->word[2], &dummy);
	b = _subborrow_u64(b, op1->word[3], op2->word[3], &dummy);
	
	return (bool)b;
}
