/**
 * Copyright (C) Takuo Hada 2016
 * @author t.hada 2015/10/04
 */
#pragma once

#include <stdint.h>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#include <magica/decimal/mgcompat.h>

static inline uint8_t mg_uint32_add(uint8_t carryIn, uint32_t op1, uint32_t op2, uint32_t *ret)
{
#if defined(_MSC_VER)
	return _addcarry_u32(carryIn, op1, op2, ret);
#else
	uint64_t n = (uint64_t)op1 + (uint64_t)op2 + carryIn;
	*ret = (uint32_t)n;
	return (uint8_t)(n >> 32) & 0x1;
#endif
}

static inline uint8_t mg_uint32_sub(uint8_t carryIn, uint32_t op1, uint32_t op2, uint32_t *ret)
{
#if defined(_MSC_VER)
	return _subborrow_u32(carryIn, op1, op2, ret);
#else
	uint64_t n = (uint64_t)op1 - (uint64_t)op2 - carryIn;
	*ret = (uint32_t)n;
	return (uint8_t)(n >> 32) & 0x1;
#endif
}

static inline uint32_t mg_uint32_mul(uint32_t op1, uint32_t op2, uint32_t *highRet)
{
#if defined(_MSC_VER)
	uint64_t n = __emulu(op1, op2);
	*highRet = (uint32_t)(n >> 32);
	return (uint32_t)n;
#else
	uint64_t n = (uint64_t)op1 * (uint64_t)op2;
	*highRet = (uint64_t)(n >> 32);
	return (uint64_t)n;
#endif
}
