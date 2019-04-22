#pragma once

#include <magica/arch/def.h>

static inline void mg_uint256_add128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	asm volatile (
		".intel_syntax noprefix\n"
		"mov eax, [ecx] \n"
		"add eax, [edx] \n"
		"mov [ecx], eax \n"
		"mov eax, [ecx+4] \n"
		"adc eax, [edx+4] \n"
		"mov [ecx+4], eax \n"
		"mov eax, [ecx+8] \n"
		"adc eax, [edx+8] \n"
		"mov [ecx+8], eax \n"
		"mov eax, [ecx+12] \n"
		"adc eax, [edx+12] \n"
		"mov [ecx+12], eax \n"
		: "+r"(op1)
		: "c"(op1), "d"(op2)
		: "memory", "eax"
	);
}
