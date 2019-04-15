#pragma once

#include <stdbool.h>

static inline bool mg_uint256_less_than(const mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char b;
	unsigned int dummy;
	
	b = _subborrow_u32(0, op1->word[0], op2->word[0], &dummy);
	b = _subborrow_u32(b, op1->word[1], op2->word[1], &dummy);
	b = _subborrow_u32(b, op1->word[2], op2->word[2], &dummy);
	b = _subborrow_u32(b, op1->word[3], op2->word[3], &dummy);
	b = _subborrow_u32(b, op1->word[4], op2->word[4], &dummy);
	b = _subborrow_u32(b, op1->word[5], op2->word[5], &dummy);
	b = _subborrow_u32(b, op1->word[6], op2->word[6], &dummy);
	b = _subborrow_u32(b, op1->word[7], op2->word[7], &dummy);
	
	return (bool)b;
}
