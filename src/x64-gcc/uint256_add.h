#pragma once

#include <stdbool.h>

typedef struct mg_uint256
{
	unsigned long long word[4];
} mg_uint256_t;

static inline void mg_uint256_add(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	register unsigned long long a2 asm("r10") = op1->word[2];
	register unsigned long long a3 asm("r11") = op1->word[3];
	
	asm volatile (
		"addq (%%rdx), %%r8 \n\t"
		"adcq 8(%%rdx), %%r9 \n\t"
		"adcq 16(%%rdx), %%r10 \n\t"
		"adcq 24(%%rdx), %%r11 \n\t"
		: "+r"(a0), "+r"(a1), "+r"(a2), "+r"(a3)
		: "d"(op2), "r"(a0), "r"(a1), "r"(a2), "r"(a3)
		: 
	);
	
	op1->word[0] = a0;
	op1->word[1] = a1;
	op1->word[2] = a2;
	op1->word[3] = a3;
}

static inline bool mg_uint256_sub(mg_uint256_t *op1, const mg_uint256_t *op2)
{
	register unsigned long long a0 asm("r8") = op1->word[0];
	register unsigned long long a1 asm("r9") = op1->word[1];
	register unsigned long long a2 asm("r10") = op1->word[2];
	register unsigned long long a3 asm("r11") = op1->word[3];
	register unsigned long long ret;
	
	asm volatile (
		"subq (%%rdx), %%r8 \n\t"
		"sbbq 8(%%rdx), %%r9 \n\t"
		"sbbq 16(%%rdx), %%r10 \n\t"
		"sbbq 24(%%rdx), %%r11 \n\t"
		"mov $0, %%eax \n\t"
		"setb %%al \n\t"
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

static inline bool mg_uint256_less_than(mg_uint256_t *op1, const mg_uint256_t *op2)
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
		"movq $0, %%eax \n\t"
		"setb %%al \n\t"
		: "=&a"(ret)
		: "d"(op2), ""r"(a0), "r"(a1), "r"(a2), "r"(a3)
		:"rax"
	);
	
	return ret != 0;
}
