#pragma once

#include <limits.h>

static inline unsigned char _uadd(unsigned char carry, unsigned int op1, unsigned int op2, unsigned int *res)
{
	unsigned char c = 0;
	c += (op1 > UINT_MAX - op2);
	*res = op1 + op2;
	c += (*res > UINT_MAX - carry);
	*res += carry;
	return c;
}

static inline unsigned char _usub(unsigned char borrow, unsigned int op1, unsigned int op2, unsigned int *res)
{
	unsigned char b = 0;
	b += (op1 < op2);
	*res = op1 - op2;
	b += (*res < borrow);
	*res -= borrow;
	return b;
}

static inline unsigned int _umul(unsigned int op1, unsigned int op2, unsigned int *high)
{
	unsigned long long res = (unsigned long long)op1 * (unsigned long long)op2;
	
	*high = (unsigned int)(res >> 32);
	return (unsigned int)(res & 0xffffffffULL);
}
