/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <NumberTypes.h>
#include <optional>
#include <string>


class FilesystemUtils
{
public:
   static bool ensurePathExists(const std::string& _pPath);
   static std::string getFilenameNoExt(const std::string& _pFilePath);

   /** Contains the '.' if available */
   static std::string getFileExt(const std::string& _pFilePath);
   static bool pathExists(const std::string& _pPath);

   static bool saveStringToFile(const std::string& _pString, const std::string& _pFilePath);
   static std::string loadStringFromFile(const std::string& _pFilePath);

private:
};
