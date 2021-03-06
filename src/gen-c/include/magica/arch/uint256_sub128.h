#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_sub128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char borrow = 0;
	
	for(int i = 0; i < mgUINT256_WORD_COUNT / 2; i++) {
		borrow = _usub(borrow, op1->word[i], op2->word[i], &op1->word[i]);
	}
	
	return borrow != 0;
}
