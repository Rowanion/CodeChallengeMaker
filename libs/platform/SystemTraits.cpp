/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "SystemTraits.h"

namespace platform {

////////////////////////////////////////////////////////////////////////////////
// compiler specific

constexpr ECompiler compiler()
{
   constexpr ECompiler compilerType
   {
#if COMPILER_MINGW
      ECompiler::MINGW
#elif COMPILER_GCC
      ECompiler::GCC
#elif COMPILER_CLANG
      ECompiler::CLANG
#elif COMPILER_MSVC
      ECompiler::MSVC
#else
      ECompiler::UNKNOWN
#endif
   };

   static_assert(compilerType != ECompiler::UNKNOWN, "Unsupported compiler!");

   return compilerType;
}

const std::string& compilerStr()
{
   static const std::string compilerName{COMPILER_NAME};
   return compilerName;
}

const std::string& compilerVersionStr()
{
   static const std::string compilerVersion{COMPILER_VERSION};
   return compilerVersion;
}

constexpr bool hasClangCompiler()
{
   constexpr bool bHasCompiler{compiler() == ECompiler::CLANG};
   return bHasCompiler;
}

constexpr bool hasGCCCompiler()
{
   constexpr bool bHasCompiler{compiler() == ECompiler::GCC};
   return bHasCompiler;
}

constexpr bool hasMinGwCompiler()
{
   constexpr bool bHasCompiler{compiler() == ECompiler::MINGW};
   return bHasCompiler;
}

constexpr bool hasMSVCCompiler()
{
   constexpr bool bHasCompiler{compiler() == ECompiler::MSVC};
   return bHasCompiler;
}

////////////////////////////////////////////////////////////////////////////////
// os specific

std::string getEnvVar(const std::string& _pEnvVar)
{
   return OS::getEnvVar(_pEnvVar);
}

void setConsoleToUtf8()
{
   OS::setConsoleToUtf8();
}

std::string getHomePath()
{
   return OS::getHomePath();
}

std::string getApplicationPath()
{
   return OS::getApplicationPath();
}

std::string getExecutablePath()
{
   return OS::getExecutablePath();
}

std::string getAppConfigLocation(const std::string& _pApplicationName)
{
   return OS::getAppConfigLocation(_pApplicationName);
}

std::string getLoginName()
{
   return OS::getLoginName();
}

std::string getTmpPath()
{
   return OS::getTmpPath();
}

size_t getNativeThreadId()
{
   return OS::getNativeThreadId();
}

int32_t makePathNative(std::string& rPath)
{
   return OS::makePathNative(rPath);
}

bool isDebuggerAttached()
{
   return OS::isDebuggerAttached();
}

///////////////////////////////////////////////////////////////////////////
// Compiler stuff
uint16_t getRevByteOrder(uint16_t val)
{
   return COMPILER::getRevByteOrder(val);
}

uint32_t getRevByteOrder(uint32_t val)
{
   return COMPILER::getRevByteOrder(val);
}

uint64_t getRevByteOrder(uint64_t val)
{
   return COMPILER::getRevByteOrder(val);
}

void revByteOrder(uint16_t& val)
{
   COMPILER::revByteOrder(val);
}

void revByteOrder(uint32_t& val)
{
   COMPILER::revByteOrder(val);
}

void revByteOrder(uint64_t& val)
{
   COMPILER::revByteOrder(val);
}

}  // namespace platform