/**
 * Error 2 string
 *
 * \file    inc/libklug/error/error2string.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <array>
#include <string_view>
#include "error.hpp"

namespace err {

constexpr std::string_view error2string(Error e) {
  using std::operator""sv;
  switch (e) {
    case Error::none: return "No Error"sv;
    case Error::format: return "Format Error"sv;
    default: return "Non-listed or unknown Error"sv;
  }
}

} // namespace err
