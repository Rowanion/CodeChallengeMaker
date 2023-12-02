/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "GCCCompiler.h"

namespace compiler_gcc {

void revByteOrder(uint16_t& val)
{
   val = __builtin_bswap16(val);
}

void revByteOrder(uint32_t& val)
{
   val = __builtin_bswap32(val);
}

void revByteOrder(uint64_t& val)
{
   val = __builtin_bswap64(val);
}

uint16_t getRevByteOrder(uint16_t val)
{
   return __builtin_bswap16(val);
}

uint32_t getRevByteOrder(uint32_t val)
{
   return __builtin_bswap32(val);
}

uint64_t getRevByteOrder(uint64_t val)
{
   return __builtin_bswap64(val);
}

}  // namespace compiler_gcc