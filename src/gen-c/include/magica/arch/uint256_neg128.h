#pragma once

#include <magica/arch/def.h>

static inline void mg_uint256_neg128(mg_uint256_t *op1)
{
	for(int i = 0; i < mgUINT256_WORD_COUNT / 2; i++) {
		op1->word[i] = ~op1->word[i];
	}
	
	unsigned char carry = 1;
	for(int i = 0; i < mgUINT256_WORD_COUNT / 2; i++) {
		carry = _uadd(carry, op1->word[i], 0, &op1->word[i]);
	}
}
