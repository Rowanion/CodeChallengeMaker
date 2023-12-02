/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "AppConfig.h"

const std::string& AppConfig::getApplicationName()
{
   static const std::string applicationName{"CodeChallengeMaker"};
   return applicationName;
}
