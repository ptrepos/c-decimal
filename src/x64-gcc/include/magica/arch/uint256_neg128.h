#pragma once

#include <magica/arch/def.h>

static inline void mg_uint256_neg128(mg_uint256_t *op1)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	
	asm volatile (
		".intel_syntax noprefix\n"
		"not r8 \n\t"
		"not r9 \n\t"
		"add r8, 1 \n\t"
		"adc r9, 0 \n\t"
		: "+r"(a0), "+r"(a1)
		: "r"(a0), "r"(a1)
		:
	);
	
	op1->word[0] = a0;
	op1->word[1] = a1;
}
