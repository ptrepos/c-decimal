#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_sub(mg_uint256_t *op1, const mg_uint256_t *op2)
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
		"mov eax, [ecx+16] \n"
		"sbb eax, [edx+16] \n"
		"mov [ecx+16], eax \n"
		"mov eax, [ecx+24] \n"
		"sbb eax, [edx+24] \n"
		"mov [ecx+24], eax \n"
		"mov eax, [ecx+28] \n"
		"sbb eax, [edx+28] \n"
		"mov [ecx+28], eax \n"
		"mov eax, 0\n"
		"setb el"
		: "+a"(ret)
		: "c"(op1), "d"(op2)
		: "memory"
	);
}
