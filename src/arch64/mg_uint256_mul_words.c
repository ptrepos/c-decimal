/**
* Copyright (C) Takuo Hada 2016
* @author t.hada 2016/07/09
*
* 256 bit integer for decimal.
* this source includes decimal implementation.
*/
#include "arch_priv_uint256.h"

MG_PRIVATE int mg_uint256_mul_words(const mg_uint256 *op1, int op1_words, const mg_uint256 *op2, int op2_words, /*out*/mg_uint256 *ret)
{
	if(op1_words <= 2 && op2_words <= 2) {
		mg_uint256_mul128(op1, op2, /*out*/ret);
		return 0;
	} else if(op2_words <= 1) {
		return mg_uint256_mul256x64(op1, op2, /*out*/ret);
	} else if(op1_words <= 1) {
		return mg_uint256_mul256x64(op2, op1, /*out*/ret);
	}

	uint8_t carry, carry2;
	uint64_t lo, hi;
	uint64_t buf[MG_UINT256_WORD_COUNT*2+1] = {0};

	for(int j = 0; j < op2_words; j++) {
		carry2 = 0;
		for(int i = 0; i < op1_words; i++) {
			int k = i + j;
			lo = mg_uint64_mul(op1->word[i], op2->word[j], &hi);

			carry = mg_uint64_add(0, buf[k], lo, &buf[k]);
			carry = mg_uint64_add(carry, buf[k+1], hi, &buf[k+1]);
			carry2 = mg_uint64_add(carry, buf[k+2], carry2, &buf[k+2]);
		}
	}

	if((buf[4] | buf[5] | buf[6] | buf[7]) != 0) {
		return 1;
	}

	memcpy(ret->word, buf, sizeof(buf[0]) * MG_UINT256_WORD_COUNT);
	return 0;
}
