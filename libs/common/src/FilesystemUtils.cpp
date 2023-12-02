/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "FilesystemUtils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

bool FilesystemUtils::ensurePathExists(const std::string& _pPath)
{
   std::filesystem::path path(_pPath);
   if(std::filesystem::exists(path))
   {
      return true;
   }

   return std::filesystem::create_directories(_pPath);
}

std::string FilesystemUtils::getFilenameNoExt(const std::string& _pFilePath)
{
   std::filesystem::path p(_pFilePath);
   return p.stem().string();
}

std::string FilesystemUtils::getFileExt(const std::string& _pFilePath)
{
   std::filesystem::path p(_pFilePath);
   return p.extension().string();
}

bool FilesystemUtils::pathExists(const std::string& _pPath)
{
   return std::filesystem::exists(std::filesystem::path(_pPath));
}

bool FilesystemUtils::saveStringToFile(const std::string& _pString, const std::string& _pFilePath)
{
   std::cout << _pFilePath << std::endl;
   std::ofstream fileStream(_pFilePath, std::ios_base::out);

   if(!fileStream.is_open())
   {
      return false;
   }
   fileStream << _pString;
   fileStream.close();
   return true;
}

std::string FilesystemUtils::loadStringFromFile(const std::string& _pFilePath)
{
   std::stringstream buffer;
   std::ifstream fileStream(_pFilePath, std::ios_base::in);
   if(!fileStream.is_open())
   {
      return std::string();
   }
   buffer << fileStream.rdbuf();
   fileStream.close();
   return buffer.str();
}
