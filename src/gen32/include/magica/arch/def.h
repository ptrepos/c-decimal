/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/07/09
 *
 * 256 bit integer for decimal.
 * this source includes decimal implementation.
 */
#pragma once

#include <memory.h>
#include <assert.h>

#define mgUINT256_WORD_COUNT	(8)
#define mgUINT256_WORD_BITS		(32)

/**
 * 256bit integer implementation for 32bit.
 */
typedef struct mg_uint256
{
	unsigned int word[mgUINT256_WORD_COUNT];
} mg_uint256_t;
