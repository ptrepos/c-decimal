/**
* Copyright (C) Takuo Hada 2016
* @author t.hada 2016/07/09
*
* 256 bit integer for decimal.
* this source includes decimal implementation.
*/
#include "arch_priv_uint256.h"

#define _JMP(op1, op2) ((op1) * 5 + (op2))

MG_PRIVATE int mg_uint256_mul_words(const mg_uint256_t *op1, int op1_words, const mg_uint256_t *op2, int op2_words, /*out*/mg_uint256_t *ret)
{
	uint64_t hi, lo;

	switch(_JMP(op1_words, op2_words)) {
	case _JMP(0, 0):
	case _JMP(0, 1):
	case _JMP(0, 2):
	case _JMP(0, 3):
	case _JMP(0, 4):
	case _JMP(1, 0):
	case _JMP(2, 0):
	case _JMP(3, 0):
	case _JMP(4, 0):
		mg_uint256_set_zero(ret);
		return 0;
	case _JMP(1, 1):
		lo = mg_uint64_mul(op1->word[0], op2->word[0], &hi);
		ret->word[3] = 0;
		ret->word[2] = 0;
		ret->word[1] = hi;
		ret->word[0] = lo;
		return 0;
	case _JMP(1, 2):
	case _JMP(2, 1):
	case _JMP(2, 2):
		mg_uint256_mul128(op1, op2, ret);
		return 0;
	case _JMP(1, 3):
	case _JMP(1, 4):
		return mg_uint256_mul256x64(op2, op1, /*out*/ret);
	case _JMP(3, 1):
	case _JMP(4, 1):
		return mg_uint256_mul256x64(op1, op2, /*out*/ret);
	default:
		break;
	}

	uint8_t carry, carry2;
	uint64_t buf[mgUINT256_WORD_COUNT*2+1] = {0};

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

	memcpy(ret->word, buf, sizeof(buf[0]) * mgUINT256_WORD_COUNT);
	return 0;
}
