/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "MSVCCompiler.h"

#include <stdlib.h>

namespace compiler_msvc {

void revByteOrder(uint16_t& val)
{
   val = _byteswap_ushort(val);
}

void revByteOrder(uint32_t& val)
{
   val = _byteswap_ulong(val);
}

void revByteOrder(uint64_t& val)
{
   val = _byteswap_uint64(val);
}

uint16_t getRevByteOrder(uint16_t val)
{
   return _byteswap_ushort(val);
}

uint32_t getRevByteOrder(uint32_t val)
{
   return _byteswap_ulong(val);
}

uint64_t getRevByteOrder(uint64_t val)
{
   return _byteswap_uint64(val);
}

}  // namespace compiler_msvc