/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <optional>
#include <system_error>

template <typename T>
struct RetVal
{
   std::error_code errCode{};
   std::optional<T> val{};

   explicit RetVal(std::error_code errCode) : errCode(errCode), val(std::nullopt) {}
   explicit RetVal(T valRef) : val(std::optional<T>(valRef)) {}
   explicit RetVal(std::optional<T>&& valOpt) : val(valOpt){};
   inline bool hasValue() const { return val.has_value(); }
   inline explicit operator bool() const { return (!errCode && val.has_value()); }
};
