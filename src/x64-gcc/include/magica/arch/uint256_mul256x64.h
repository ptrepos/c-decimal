#pragma once

#include <stdbool.h>
#include <magica/arch/def.h>

static inline bool mg_uint256_mul256x64(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	register unsigned long long a2 asm("r10") = op1->word[2];
	register unsigned long long a3 asm("r11") = op1->word[3];
	register unsigned long long r0 asm("r12");
	register unsigned long long r1 asm("r13");
	register unsigned long long r2 asm("r14");
	register unsigned long long r3 asm("r15");
	unsigned long long ovf1;
	unsigned long long ovf2;
	
	asm volatile (
		".intel_syntax noprefix\n"
		
		"xor rbx, rbx \n"
		
		"mov rax, r8 \n"
		"mul rsi \n"
		"mov r12, rax \n"
		"mov r13, rdx \n"
		
		"mov rax, r10 \n"
		"mul rsi \n"
		"mov r14, rax \n"
		"mov r15, rdx \n"
		
		"mov rax, r9 \n"
		"mul rsi \n"
		"add r13, rax \n"
		"adc r14, rdx \n"
		"adc r15, 0 \n"
		"adc rbx, 0 \n"     /* get carry flag */
		
		"mov rax, r11 \n"
		"mul rsi \n"
		"add r15, rax \n"
		"adc rbx, 0 \n"     /* get carry flag */
		: "+r"(r0), "+r"(r1), "+r"(r2), "+r"(r3), "+d"(ovf1), "+b"(ovf2)
		: "r"(a0), "r"(a1), "r"(a2), "r"(a3), "S"(op2->word[0])
		: "rax"
	);
	
	op1->word[0] = r0;
	op1->word[1] = r1;
	op1->word[2] = r2;
	op1->word[3] = r3;
	
	return (ovf1 | ovf2) != 0;
}
