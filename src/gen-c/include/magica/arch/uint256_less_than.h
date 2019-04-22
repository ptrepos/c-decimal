#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_less_than(const mg_uint256_t *op1, const mg_uint256_t *op2)
{
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		if (op1->word[i] != op2->word[i])
			return op1->word[i] < op2->word[i];
	}
	return false;
}
