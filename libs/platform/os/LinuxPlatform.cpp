/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "LinuxPlatform.h"

#include <linux/limits.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <ErrnoGuard.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <pwd.h>
#include <unistd.h>

/// \namespace os_linux
namespace os_linux {

std::string getEnvVar(const std::string& pEnvVar)
{
   const char* envValue = std::getenv(pEnvVar.c_str());
   if(envValue == nullptr)
   {
      struct passwd pw;
      struct passwd* pwPtr;
      char buf[1024];

      const int res       = getpwuid_r(getuid(), &pw, buf, sizeof buf, &pwPtr);
      const bool bSuccess = (res == 0);
      if(!bSuccess)
      {
         return std::string();
      }

      return std::string(pw.pw_dir);
   }
   return std::string(envValue);
}

FileSystemEncoded makeFileSystemEncoded(const std::string& pPath)
{
   return pPath;
}

void setConsoleToUtf8() {}

std::string getHomePath()
{
   return getEnvVar("HOME");
}

std::string getApplicationPath()
{
   auto pathStr = getExecutablePath();
   std::filesystem::path p(pathStr.c_str());
   return p.parent_path().string();
}

std::string getExecutablePath()
{
   std::string pathStr;
   char buff[PATH_MAX];
   ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
   if(len != -1)
   {
      buff[len] = '\0';
      pathStr   = buff;
   }
   std::filesystem::path p(pathStr.c_str());
   return p.string();
}

std::string getAppConfigLocation(const std::string& _pApplicationName)
{
   std::filesystem::path p(getHomePath());
   p = p / ".config" / _pApplicationName;
   if(!std::filesystem::exists(p))
   {
      std::filesystem::create_directories(p);
   }

   return p.string();
}

int32_t makePathNative(std::string& rPath)
{
   uint32_t err = 0;

   std::filesystem::path p1(rPath.c_str());
   std::replace(rPath.begin(), rPath.end(), '\\', '/');

   if(rPath.at(1) == ':')
   {
      std::string noRoot(rPath.substr(2, rPath.size()));
      rPath = noRoot;
   }

   return static_cast<int32_t>(err);
}

std::string getLoginName()
{
   return getEnvVar("USER");
}

std::string getTmpPath()
{
   return "/tmp";
}

size_t getNativeThreadId()
{
   return static_cast<size_t>(syscall(SYS_gettid));
}

void localtimeSafe(const time_t* pTimeStamp, tm* result)
{
   localtime_r(pTimeStamp, result);
}

bool isDebuggerAttached()
{
   // Libstdc++ has a bug, where std::ifstream sets errno to 0
   // This way our users can properly assert over errno values
   platform::ErrnoGuard guard;
   std::ifstream in("/proc/self/status");
   for(std::string line; std::getline(in, line);)
   {
      static const int PREFIX_LEN = 11;
      if(line.compare(0, PREFIX_LEN, "TracerPid:\t") == 0)
      {
         // We're traced if the PID is not 0 and no other PID starts
         // with 0 digit, so it's enough to check for just a single
         // character.
         return line.length() > PREFIX_LEN && line[PREFIX_LEN] != '0';
      }
   }

   return false;
}
}  // namespace os_linux
