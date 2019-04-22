#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>
#include <magica/arch/uintbase.h>

static inline bool mg_uint256_sub(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned char borrow = 0;
	
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		borrow = _usub(borrow, op1->word[i], op2->word[i], &op1->word[i]);
	}

	return borrow != 0;
}
