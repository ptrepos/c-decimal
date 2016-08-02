/**
 * Copyright (C) dachicraft foundation 2015
 * @author t.hada 2015/10/27
 */
#pragma once

#ifdef NDEBUG

#include<stdlib.h>

#define mg_assert(expression)	if(!(expression)) abort()

#else

#include<assert.h>

#define mg_assert(expression)	assert(expression)

#endif
