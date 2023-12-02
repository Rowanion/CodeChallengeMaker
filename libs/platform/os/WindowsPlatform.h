/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <cstdint>
#include <string>

using FileSystemEncoded = std::wstring;

/** @namespace os_windows */
namespace os_windows {

std::string getEnvVar(const std::string& _pEnvVar);
void setConsoleToUtf8();
FileSystemEncoded makeFileSystemEncoded(const std::string& _pPath);
std::string getHomePath();
std::string getApplicationPath();
std::string getExecutablePath();
std::string getAppConfigLocation(const std::string& _pApplicationName);
int32_t makePathNative(std::string& _rPath);
std::string getLoginName();
std::string getTmpPath();
size_t getNativeThreadId();

///////////////////////////////////////////////////////////////////////////////////////
// local native functions /////////////////////////////////////////////////////////////
/**
 * @brief Converts the given path to a long path name. Windows shenanigans.
 *
 * This is required for most paths returned directly from windows API.
 * I.e converts C:\PROGRA~1 to C:\Program Files
 *
 * @return (string) given path name as long version or the same that went in - in case of failure.
 */
std::string convertToLongPath(const std::string& _pShortPath);

void localtimeSafe(const time_t* _pTimeStamp, tm* _rResult);

bool isDebuggerAttached();

}  // namespace os_windows