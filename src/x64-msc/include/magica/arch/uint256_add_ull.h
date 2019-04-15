#pragma once

#include <intrin.h>
#include <magica/arch/def.h>

static inline void mg_uint256_add_ull(mg_uint256_t *op1, unsigned long long op2)
{
	unsigned char c;

	c = _addcarry_u64(0, op1->word[0], op2, &op1->word[0]);
	c = _addcarry_u64(c, op1->word[1], 0, &op1->word[1]);
	c = _addcarry_u64(c, op1->word[2], 0, &op1->word[2]);
	c = _addcarry_u64(c, op1->word[3], 0, &op1->word[3]);
}
