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
		"movq %%r8, %%rax \n\t"
		"subq (%%rdx), %%rax \n\t"
		"movq %%r9, %%rax \n\t"
		"sbbq 8(%%rdx), %%rax \n\t"
		"movq %%r10, %%rax \n\t"
		"sbbq 16(%%rdx), %%rax \n\t"
		"movq %%r11, %%rax \n\t"
		"sbbq 24(%%rdx), %%rax \n\t"
		"movl $0, %%eax \n\t"
		"setb %%al \n\t"
		: "=&a"(ret)
		: "d"(op2), "r"(a0), "r"(a1), "r"(a2), "r"(a3)
		:
	);
	
	return ret != 0;
}
