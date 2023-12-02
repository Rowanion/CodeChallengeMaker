/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "Utils.h"

#include <iomanip>
#include <sstream>

std::string Utils::byteSizeToString(int64 _pByteSize)
{
   static constexpr int64 B  = 1;
   static constexpr int64 KB = 1024;
   static constexpr int64 MB = KB * 1024;
   static constexpr int64 GB = MB * 1024;
   static constexpr int64 TB = GB * 1024;

   std::string suffix;
   const int64* divisor = nullptr;

   if(_pByteSize < KB)
   {
      suffix  = " B";
      divisor = &B;
   }
   else if(_pByteSize < MB)
   {
      suffix  = " KB";
      divisor = &KB;
   }
   else if(_pByteSize < GB)
   {
      suffix  = " MB";
      divisor = &MB;
   }
   else if(_pByteSize < TB)
   {
      suffix  = " GB";
      divisor = &GB;
   }
   else
   {
      suffix  = " TB";
      divisor = &TB;
   }

   const float percentFloat = _pByteSize / static_cast<float>(*divisor);

   std::stringstream stream;
   stream << std::fixed << std::setprecision(2) << percentFloat;
   return stream.str() + suffix;
}
