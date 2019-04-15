#pragma once

static inline void mg_uint256_neg(mg_uint256_t *op1)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	register unsigned long long a2 asm("r10") = op1->word[2];
	register unsigned long long a3 asm("r11") = op1->word[3];
	register unsigned long long ret;
	
	asm volatile (
		"notq %%r8 \n\t"
		"notq %%r9 \n\t"
		"notq %%r10 \n\t"
		"notq %%r11 \n\t"
		"addq $1, %%r8 \n\t"
		"adcq $0, %%r9 \n\t"
		"adcq $0, %%r10 \n\t"
		"adcq $0, %%r11 \n\t"
		: "+r"(a0), "+r"(a1), "+r"(a2), "+r"(a3)
		: ""r"(a0), "r"(a1), "r"(a2), "r"(a3)
		:
	);
	
	op1->word[0] = a0;
	op1->word[1] = a1;
	op1->word[2] = a2;
	op1->word[3] = a3;
}
