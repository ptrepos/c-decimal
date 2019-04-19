#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_sub(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	register unsigned long long a2 asm("r10") = op1->word[2];
	register unsigned long long a3 asm("r11") = op1->word[3];
	register unsigned long long ret;
	
	asm volatile (
		".intel_syntax noprefix\n"
		"sub r8, [rdx] \n\t"
		"sbb r9, 8[rdx] \n\t"
		"sbb r10, 16[rdx] \n\t"
		"sbb r11, 24[rdx] \n\t"
		"mov eax, 0 \n\t"
		"setb al \n\t"
		: "+r"(a0), "+r"(a1), "+r"(a2), "+r"(a3), "=a"(ret)
		: "d"(op2), "r"(a0), "r"(a1), "r"(a2), "r"(a3)
		: 
	);
	
	op1->word[0] = a0;
	op1->word[1] = a1;
	op1->word[2] = a2;
	op1->word[3] = a3;
	
	return ret;
}
