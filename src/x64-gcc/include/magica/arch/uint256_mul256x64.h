#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

/**
 * overflow checked multiple for digits of word.
 */
bool mg_uint256_mul_digits(const mg_uint256_t *op1, int op1_digits, const mg_uint256_t *op2, int op2_digits, mg_uint256_t *result);

static inline bool mg_uint256_mul256x64(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	int op1_digits = mgUINT256_WORD_COUNT;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;
	
	return mg_uint256_mul_digits(op1, op1_digits, op2, 1, op1);
}
