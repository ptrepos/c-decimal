#pragma once

#include <magica/arch/def.h>
#include <magica/arch/uintbase.h>

static inline void mg_uint256_add128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char carry = 0;
	
	for(int i = 0; i < mgUINT256_WORD_COUNT / 2; i++) {
		carry = _uadd(carry, op1->word[i], op2->word[i], &op1->word[i]);
	}
}
