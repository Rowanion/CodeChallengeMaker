/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */
 
 #pragma once

#include <string>

class ErrorCode
{
public:
   ErrorCode(){};
   ErrorCode(const std::string& _rMessage) { m_errorMsg = _rMessage; };
   inline bool hasError() const { return m_errorMsg.empty(); }
   inline const std::string& errorMsg() const { return m_errorMsg; }
   inline explicit operator bool() const { return hasError(); }

private:
   std::string m_errorMsg;
};
