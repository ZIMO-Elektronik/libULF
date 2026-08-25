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
    case Error::ok: return "No Error"sv;
    case Error::format: return "Format Error"sv;
    case Error::nak: return "Nak Received"sv;
    case Error::crc: return "Bad CRC"sv;
    case Error::usb: return "USB Backend error"sv;
    default: return "Non-listed or unknown Error"sv;
  }
}

} // namespace err
