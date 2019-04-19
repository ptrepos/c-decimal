#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_sub128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	register unsigned long long ret;
	
	asm volatile (
		".intel_syntax noprefix\n"
		"sub r8, [rdx] \n\t"
		"sbb r9, 8[rdx] \n\t"
		"mov eax, 0 \n\t"
		"setb al \n\t"
		: "+r"(a0), "+r"(a1), "=a"(ret)
		: "d"(op2), "r"(a0), "r"(a1)
		: 
	);
	
	op1->word[0] = a0;
	op1->word[1] = a1;
	
	return ret;
}
