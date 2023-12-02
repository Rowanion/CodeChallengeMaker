/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <cstdint>
#include <string>

// unified platform ident
// include platform specific headers
#if OS_WINDOWS
#define OS os_windows
#include <os/WindowsPlatform.h>
#elif OS_LINUX
#define OS os_linux
#include <os/LinuxPlatform.h>
#else
static_assert(false, "Unsupported operating system!");
#endif

// unified compiler identifier
// include compiler specific headers
#if COMPILER_GCC
#define COMPILER compiler_gcc
#include <compiler/GCCCompiler.h>
#elif COMPILER_MSVC
#define COMPILER compiler_msvc
#include <compiler/MSVCCompiler.h>
#elif COMPILER_CLANG
#define COMPILER compiler_clang
#include <compiler/ClangCompiler.h>
#else
static_assert(false, "Unsupported compiler!");
#endif

/** @namespace platform */
namespace platform {

////////////////////////////////////////////////////////////////////////////////
// compiler specific
enum class ECompiler : uint8_t
{
   UNKNOWN = 0,
   GCC,
   CLANG,
   MINGW,
   MSVC
};

constexpr ECompiler compiler();
const std::string& compilerStr();
const std::string& compilerVersionStr();
constexpr bool hasClangCompiler();
constexpr bool hasGCCCompiler();
constexpr bool hasMinGwCompiler();
////////////////////////////////////////////////////////////////////////////////
// os specific
enum class EOs : uint8_t
{
   UNKNOWN = 0,
   LINUX,
   WINDOWS
};

constexpr EOs operatingSystem()
{
   constexpr EOs osType
   {
#if OS_LINUX
      EOs::LINUX
#elif OS_WINDOWS
      EOs::WINDOWS
#else
      EOs::UNKNOWN
#endif
   };

   static_assert(osType != EOs::UNKNOWN, "Unsupported operating System!");

   return osType;
}

const std::string& osStr()
{
   static const std::string osName{OS_NAME};
   return osName;
};
constexpr bool isLinuxOS()
{
   constexpr bool bIsLinux{operatingSystem() == EOs::LINUX};
   return bIsLinux;
}
constexpr bool isWindowsOS()
{
   constexpr bool bIsWindows{operatingSystem() == EOs::WINDOWS};
   return bIsWindows;
}

/**
 * @brief Return the value of a given environment variable.
 *
 * @return (string) Value of environment var pEnvVar
 */
std::string getEnvVar(const std::string& _pEnvVar);

/**
 * @brief Enables Utf8 capabilities of console.
 * This is a win-specific beast which does nothing on other systems
 */
void setConsoleToUtf8();

/**
 * @brief Returns the user's home path
 *
 * @return (wstring) the user's home path
 */
std::string getHomePath();

/**
 * @brief Returns the absolute path to the executable
 *
 * @return (wstring) the absolute base path
 */
std::string getApplicationPath();

/**
 * @brief Returns the absolute path of the executable
 *
 * @return (wstring) the absolute base path
 */
std::string getExecutablePath();

std::string getAppConfigLocation(const std::string& _pApplicationName);

/**
 * @brief Returns the system's login name of executing user.
 *
 * @return (string) The user name / login name.
 */
std::string getLoginName();

/**
 * @brief Returns the user's system-native path to temp folder.
 *
 * @return (string) Path to system's temp folder.
 */
std::string getTmpPath();

/**
 * @brief Returns the thread ID of calling thread.
 *
 * @return (size_t) Thread ID of calling thread.
 */
size_t getNativeThreadId();

int32_t makePathNative(std::string& rPath);

bool isDebuggerAttached();

uint16_t getRevByteOrder(uint16_t val);
uint32_t getRevByteOrder(uint32_t val);
uint64_t getRevByteOrder(uint64_t val);

void revByteOrder(uint16_t& val);
void revByteOrder(uint32_t& val);
void revByteOrder(uint64_t& val);
}  // namespace platform