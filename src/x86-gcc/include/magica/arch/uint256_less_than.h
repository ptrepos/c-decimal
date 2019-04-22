#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_less_than(const mg_uint256_t *op1, const mg_uint256_t *op2)
{
	register unsigned long long ret;
	
	asm volatile (
		".intel_syntax noprefix\n"
		"mov eax, [rcx] \n\t"
		"sub eax, [rdx] \n\t"
		"mov eax, 4[rcx] \n\t"
		"sbb eax, 4[rdx] \n\t"
		"mov eax, 8[rcx] \n\t"
		"sbb eax, 8[rdx] \n\t"
		"mov eax, 12[rcx] \n\t"
		"sbb eax, 12[rdx] \n\t"
		"mov eax, 16[rcx] \n\t"
		"sbb eax, 16[rdx] \n\t"
		"mov eax, 20[rcx] \n\t"
		"sbb eax, 20[rdx] \n\t"
		"mov eax, 24[rcx] \n\t"
		"sbb eax, 24[rdx] \n\t"
		"mov eax, 28[rcx] \n\t"
		"sbb eax, 28[rdx] \n\t"
		"setb al \n\t"
		: "=&a"(ret)
		: "c"(op1), "d"(op2)
		: 
	);
	
	return ret != 0;
}
