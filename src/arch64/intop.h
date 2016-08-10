/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2015/10/04
 */
#pragma once

#include <stdint.h>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#include <magica/decimal/mgcompat.h>

static inline uint8_t mg_uint64_add(uint8_t carryIn, uint64_t op1, uint64_t op2, uint64_t *ret)
{
#if defined(_MSC_VER) && defined(_M_AMD64)
	return _addcarry_u64(carryIn, op1, op2, ret);
#elif defined(__GNUC__) || defined(__clang__)
	unsigned __int128 n = (unsigned __int128)op1 + (unsigned __int128)op2 + carryIn;
	*ret = (uint64_t)n;
	return (uint8_t)(n >> 64) & 0x1;
#endif
}

static inline uint8_t mg_uint64_sub(uint8_t carryIn, uint64_t op1, uint64_t op2, uint64_t *ret)
{
#if defined(_MSC_VER) && defined(_M_AMD64)
	return _subborrow_u64(carryIn, op1, op2, ret);
#elif defined(__GNUC__) || defined(__clang__)
	unsigned __int128 n = (unsigned __int128)op1 - (unsigned __int128)op2 - carryIn;
	*ret = (uint64_t)n;
	return (uint8_t)(n >> 64) & 0x1;
#endif
}

static inline uint64_t mg_uint64_mul(uint64_t op1, uint64_t op2, uint64_t *high_ret)
{
#if defined(_MSC_VER) && defined(_M_AMD64)
	return _umul128(op1, op2, high_ret);
#elif defined(__GNUC__) || defined(__clang__)
	unsigned __int128 n = (unsigned __int128)op1 * (unsigned __int128)op2;
	*high_ret = (uint64_t)(n >> 64);
	return (uint64_t)n;
#endif
}
