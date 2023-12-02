/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "WindowsPlatform.h"

#include <codecvt>
#include <ctime>
#include <filesystem>
#include <lmcons.h>
#include <locale>
#include <sstream>
#include <windows.h>

namespace os_windows {

std::string getEnvVar(const std::string& _pEnvVar)
{
   char* envValue;
   size_t envValueLength;

   _dupenv_s(&envValue, &envValueLength, _pEnvVar.c_str());

   const std::string returnVal(envValue);
   free(envValue);

   return std::move(returnVal);
}

void setConsoleToUtf8()
{
   SetConsoleOutputCP(CP_UTF8);
}

FileSystemEncoded makeFileSystemEncoded(const std::string& pPath)
{
   // basically a conversion from UTF8 to WString
   std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
   return converter.from_bytes(pPath);
}

std::string getHomePath()
{
   return getEnvVar("HOMEDRIVE").append(getEnvVar("HOMEPATH"));
}

std::string getApplicationPath()
{
   std::string pathStr = getExecutablePath();
   std::filesystem::path p(pathStr.c_str());
   return p.parent_path().string();
}

std::string getExecutablePath()
{
   std::vector<char> pathBuf;
   size_t copied = 0;
   do
   {
      pathBuf.resize(pathBuf.size() + MAX_PATH);
      copied = GetModuleFileName(0, &pathBuf.at(0), static_cast<DWORD>(pathBuf.size()));
   } while(copied >= pathBuf.size());

   pathBuf.resize(copied);

   std::wstring path(pathBuf.begin(), pathBuf.end());
   using convert_typeX = std::codecvt_utf8<wchar_t>;
   std::wstring_convert<convert_typeX, wchar_t> converterX;
   std::string pathStr = converterX.to_bytes(path);
   std::filesystem::path p(pathStr);
   return p.string();
}

std::string getAppConfigLocation(const std::string& _pApplicationName)
{
   std::filesystem::path p(getHomePath());
   p = p / "AppData" / "Local" / _pApplicationName;
   if(!std::filesystem::exists(p))
   {
      std::filesystem::create_directories(p);
   }

   return p.string();
}

int32_t makePathNative(std::string& rPath)
{
   int32_t err = 0;
   std::filesystem::path p(rPath.c_str());
   rPath = p.make_preferred().string();

   return err;
}

std::string getLoginName()
{
   char username[UNLEN + 1];
   DWORD username_len = UNLEN + 1;
   ::GetUserName(username, &username_len);
   return std::string(username);
}

std::string getTmpPath()
{
   DWORD dwRetVal = 0;
   TCHAR path[MAX_PATH];
   dwRetVal = ::GetTempPath(MAX_PATH, path);
   if(dwRetVal > MAX_PATH || (dwRetVal == 0))
   {
      // could not get tmp path
      return std::string();
   }
   return convertToLongPath(path);
}

size_t getNativeThreadId()
{
   return ::GetCurrentThreadId();
}

bool isDebuggerAttached()
{
   return IsDebuggerPresent();
}

///////////////////////////////////////////////////////////////////////////////////////
// local native functions /////////////////////////////////////////////////////////////
std::string convertToLongPath(const std::string& _pShortPath)
{
   DWORD retval          = 0;
   const int bufSize     = 4096;
   TCHAR buffer[bufSize] = TEXT("");

   retval = GetLongPathName(_pShortPath.c_str(), buffer, bufSize);
   if(retval == 0)
      return _pShortPath;
   return std::string(buffer);
}

void localtimeSafe(const time_t* _pTimeStamp, tm* _rResult)
{
   localtime_s(_rResult, _pTimeStamp);
}

}  // namespace os_windows
