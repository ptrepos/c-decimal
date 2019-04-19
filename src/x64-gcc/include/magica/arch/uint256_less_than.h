#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_less_than(const mg_uint256_t *op1, const mg_uint256_t *op2)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	register unsigned long long a2 asm("r10") = op1->word[2];
	register unsigned long long a3 asm("r11") = op1->word[3];
	register unsigned long long ret;
	
	asm volatile (
		".intel_syntax noprefix\n"
		"mov rax, r8 \n\t"
		"sub rax, [rdx] \n\t"
		"mov rax, r9 \n\t"
		"sbb rax, 8[rdx] \n\t"
		"mov rax, r10 \n\t"
		"sbb rax, 16[rdx] \n\t"
		"mov rax, r11 \n\t"
		"sbb rax, 24[rdx] \n\t"
		"mov eax, 0 \n\t"
		"setb al \n\t"
		: "=&a"(ret)
		: "d"(op2), "r"(a0), "r"(a1), "r"(a2), "r"(a3)
		: 
	);
	
	return ret != 0;
}
