/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <cstdint>
#include <string>

using FileSystemEncoded = std::string;

/** @namespace os_linux */
namespace os_linux {

std::string getEnvVar(const std::string& pEnvVar);
void setConsoleToUtf8();
FileSystemEncoded makeFileSystemEncoded(const std::string& pPath);
std::string getHomePath();
std::string getApplicationPath();
std::string getExecutablePath();
std::string getAppConfigLocation(const std::string& _pApplicationName);
int32_t makePathNative(std::string& rPath);
std::string getLoginName();
std::string getTmpPath();
size_t getNativeThreadId();
void localtimeSafe(const time_t* pTimeStamp, tm* result);
bool isDebuggerAttached();

}  // namespace os_linux