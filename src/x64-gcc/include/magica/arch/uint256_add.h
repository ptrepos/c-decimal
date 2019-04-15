#pragma once

#include <magica/arch/def.h>

static inline void mg_uint256_add(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	register unsigned long long a2 asm("r10") = op1->word[2];
	register unsigned long long a3 asm("r11") = op1->word[3];
	
	asm volatile (
		"addq (%%rdx), %%r8 \n\t"
		"adcq 8(%%rdx), %%r9 \n\t"
		"adcq 16(%%rdx), %%r10 \n\t"
		"adcq 24(%%rdx), %%r11 \n\t"
		: "+r"(a0), "+r"(a1), "+r"(a2), "+r"(a3)
		: "d"(op2), "r"(a0), "r"(a1), "r"(a2), "r"(a3)
		: 
	);
	
	op1->word[0] = a0;
	op1->word[1] = a1;
	op1->word[2] = a2;
	op1->word[3] = a3;
}
