/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <Globals.h>
#include <cstdint>
#include <iostream>

#if defined(NDEBUG)
#define IS_DEBUG 0
#else
#define IS_DEBUG 1
#endif

#define MSVC_PRAGMA(Pragma)
#define MSVC_DISABLE_WARNING(WARNING)

#define COMPILER_PUSH_WARNING() _Pragma("clang diagnostic push")
#define COMPILER_POP_WARNING() _Pragma("clang diagnostic push")
#define CLANG_DISABLE_WARNING(WARNING) _Pragma("clang diagnostic ignored #WARNING")

// TODO: this does not seem to be able to return
#define PLATFORM_BREAK() (__builtin_debugtrap())

#define ensure(InExpression)                                                                                           \
   if(!(InExpression))                                                                                                 \
   {                                                                                                                   \
      std::cerr << std::string(                                                                                        \
          STRINGIFY(__FILE__) ", " STRINGIFY(__LINE__) " : Error - 'ensure' failed: \"" STRINGIFY(InExpression) "\"!") \
                << std::endl;                                                                                          \
                                                                                                                       \
      if(platform::isDebuggerAttached())                                                                               \
      {                                                                                                                \
         PLATFORM_BREAK();                                                                                             \
      }                                                                                                                \
   }

namespace compiler_clang {

uint16_t getRevByteOrder(uint16_t val);
uint32_t getRevByteOrder(uint32_t val);
uint64_t getRevByteOrder(uint64_t val);

void revByteOrder(uint16_t& val);
void revByteOrder(uint32_t& val);
void revByteOrder(uint64_t& val);

}  // namespace compiler_clang