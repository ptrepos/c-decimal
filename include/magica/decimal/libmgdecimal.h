#pragma once

#include <magica/decimal/mgcompat.h>

#if defined(__MG_DECIMAL_DLL)
#	define MG_DECIMAL_API	MG_API_EXPORT
#else
#	define MG_DECIMAL_API	MG_API_IMPORT
#endif

#if defined(_M_IX86) || defined(__x86)
#	define __MG_DECIMAL_ARCH32__
#endif
