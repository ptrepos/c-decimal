#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_sub128(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	unsigned int ret;
	
	asm volatile (
		".intel_syntax noprefix\n"
		"mov eax, [ecx] \n"
		"sub eax, [edx] \n"
		"mov [ecx], eax \n"
		"mov eax, [ecx+4] \n"
		"sbb eax, [edx+4] \n"
		"mov [ecx+4], eax \n"
		"mov eax, [ecx+8] \n"
		"sbb eax, [edx+8] \n"
		"mov [ecx+8], eax \n"
		"mov eax, [ecx+12] \n"
		"sbb eax, [edx+12] \n"
		"mov [ecx+12], eax \n"
		"mov eax, 0\n"
		"setb el"
		: "+a"(ret)
		: "c"(op1), "d"(op2)
		: "memory"
	);
}
