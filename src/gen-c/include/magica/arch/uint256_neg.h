#pragma once

#include <magica/arch/def.h>

static inline void mg_uint256_neg(mg_uint256_t *op1)
{
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		op1->word[i] = ~op1->word[i];
	}
	
	unsigned char carry = 1;
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		carry = _uadd(carry, op1->word[i], 0, &op1->word[i]);
	}
}
