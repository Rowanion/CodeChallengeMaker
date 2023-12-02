/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <cstdint>
#include <Globals.h>

#if defined(_DEBUG)
#define IS_DEBUG 1
#else
#define IS_DEBUG 0
#endif

#define MSVC_PRAGMA(Pragma) __pragma(Pragma)

#define COMPILER_PUSH_WARNING() __pragma(warning(push))
#define COMPILER_POP_WARNING() __pragma(warning(pop))
#define MSVC_DISABLE_WARNING(WARNING_NUMBER) __pragma(warning(disable : WARNING_NUMBER))

// Q: Why is there a __nop() before __debugbreak()?
// A: VS' debug engine has a bug where it will silently swallow explicit
// breakpoint interrupts when single-step debugging either line-by-line or
// over call instructions. This can hide legitimate reasons to trap. Asserts
// for example, which can appear as if the did not fire, leaving a programmer
// unknowingly debugging an undefined process.
#define PLATFORM_BREAK() (__nop(), __debugbreak())

#define ensure(InExpression)                                                           \
   if(!(InExpression))                                                                 \
   {                                                                                   \
      if(!platform::isDebuggerAttached())                                              \
      {                                                                                \
         /* can't do logging in header */                                              \
         std::cerr << std::string(STRINGIFY(__FILE__) ", " STRINGIFY(                  \
             __LINE__) " : Error - 'ensure' failed: \"" STRINGIFY(InExpression) "\"!") \
                   << std::endl;                                                       \
      }                                                                                \
      else                                                                             \
      {                                                                                \
         PLATFORM_BREAK();                                                             \
      }                                                                                \
   }
namespace compiler_msvc {

uint16_t getRevByteOrder(uint16_t val);
uint32_t getRevByteOrder(uint32_t val);
uint64_t getRevByteOrder(uint64_t val);

void revByteOrder(uint16_t& val);
void revByteOrder(uint32_t& val);
void revByteOrder(uint64_t& val);

}  // namespace compiler_msvc