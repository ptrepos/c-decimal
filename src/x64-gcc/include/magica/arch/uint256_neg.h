#pragma once

#include <magica/arch/def.h>

static inline void mg_uint256_neg(mg_uint256_t *op1)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	register unsigned long long a2 asm("r10") = op1->word[2];
	register unsigned long long a3 asm("r11") = op1->word[3];
	register unsigned long long ret;
	
	asm volatile (
		".intel_syntax noprefix\n"
		"not r8 \n\t"
		"not r9 \n\t"
		"not r10 \n\t"
		"not r11 \n\t"
		"add r8, 1 \n\t"
		"adc r9, 0 \n\t"
		"adc r10, 0 \n\t"
		"adc r11, 0 \n\t"
		: "+r"(a0), "+r"(a1), "+r"(a2), "+r"(a3)
		: "r"(a0), "r"(a1), "r"(a2), "r"(a3)
		:
	);
	
	op1->word[0] = a0;
	op1->word[1] = a1;
	op1->word[2] = a2;
	op1->word[3] = a3;
}
