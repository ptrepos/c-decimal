#pragma once

#include <magica/decimal/mgcompat.h>
#include <magica/arch/def.h>
#include <magica/arch/uintbase.h>
#include <magica/arch/uint256.h>

MG_PRIVATE bool mg_uint256_mul_digits(
		const mg_uint256_t *op1, int op1_digits, 
		const mg_uint256_t *op2, int op2_digits, 
		mg_uint256_t *result)
{
	unsigned char carry;
	_mg_word_t words[mgUINT256_WORD_COUNT*2] = {0};
	
	for(int i = 0; i < op1_digits; i++) {
		if (op1->word[i] == 0)
			continue;
		
		for(int j = 0; j < op2_digits; j++) {
			if (op2->word[j] == 0)
				continue;
			
			_mg_word_t high;
			_mg_word_t low = _umul(op1->word[i], op2->word[j], &high);
			
			int k = i + j;
			carry = 0;
			carry = _uadd(carry, words[k], low, &words[k]);
			k++;
			carry = _uadd(carry, words[k], high, &words[k]);
			k++;
			
			while(carry != 0) {
				carry = _uadd(carry, words[k], 0, &words[k]);
				k++;
			}
		}
	}
	
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		result->word[i] = words[i];
	}
	
	for(int i = 0; i < mgUINT256_WORD_COUNT; i++) {
		if(words[mgUINT256_WORD_COUNT+i] != 0)
			return true;
	}
	
	return false;
}
