/**
* Copyright (C) Takuo Hada 2016
* @author t.hada 2016/07/09
*
* 256 bit integer for decimal.
* this source includes decimal implementation.
*/
#include "uint256_arch.h"

MG_PRIVATE int mg_uint256_mul_digits(const mg_uint256_t *op1, int op1_words, const mg_uint256_t *op2, int op2_words, /*out*/mg_uint256_t *ret)
{
	uint8_t carry, carry2;
	uint32_t lo, hi;
	uint32_t buf[mgUINT256_WORD_COUNT*2+1] = {0};

	for(int j = 0; j < op2_words; j++) {
		carry2 = 0;
		for(int i = 0; i < op1_words; i++) {
			int k = i + j;
			lo = mg_uint32_mul(op1->word[i], op2->word[j], &hi);

			carry = mg_uint32_add(0, buf[k], lo, &buf[k]);
			carry = mg_uint32_add(carry, buf[k+1], hi, &buf[k+1]);
			carry2 = mg_uint32_add(carry, buf[k+2], carry2, &buf[k+2]);
		}
	}

	if((buf[8] | buf[9] | buf[10] | buf[11]) != 0 || (buf[12] | buf[13] | buf[14] | buf[15]) != 0) {
		return 1;
	}

	memcpy(ret->word, buf, sizeof(buf[0]) * mgUINT256_WORD_COUNT);

	return 0;
}