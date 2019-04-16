#pragma once

#include <magica/arch/def.h>

static inline void mg_uint256_neg128(mg_uint256_t *op1)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	
	asm volatile (
		"notq %%r8 \n\t"
		"notq %%r9 \n\t"
		"addq $1, %%r8 \n\t"
		"adcq $0, %%r9 \n\t"
		: "+r"(a0), "+r"(a1)
		: "r"(a0), "r"(a1)
		:
	);
	
	op1->word[0] = a0;
	op1->word[1] = a1;
}
