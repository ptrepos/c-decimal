#pragma once

#include <intrin.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_mul256x64(
		mg_uint256_t *op1, 
		const mg_uint256_t *op2)
{
	unsigned char carry, carry2;
	unsigned long long hi, lo;
	unsigned long long word0 = 0, word1 = 0, word2 = 0, word3 = 0;

	carry2 = 0;

	lo = _umul128(op1->word[0], op2->word[0], &hi);

	word0 = lo;
	word1 = hi;

	lo = _umul128(op1->word[2], op2->word[0], &hi);

	word2 = lo;
	word3 = hi;

	lo = _umul128(op1->word[1], op2->word[0], &hi);

	carry = _addcarry_u64(0, word1, lo, &word1);
	carry = _addcarry_u64(carry, word2, hi, &word2);
	carry2 = _addcarry_u64(carry, word3, 0, &word3);
	if(carry2 != 0) {
		return true;
	}

	lo = _umul128(op1->word[3], op2->word[0], &hi);

	carry = _addcarry_u64(0, word3, lo, &word3);
	if(carry != 0 || carry2 != 0 || hi != 0) {
		return true;
	}

	op1->word[0] = word0;
	op1->word[1] = word1;
	op1->word[2] = word2;
	op1->word[3] = word3;

	return false;
}
