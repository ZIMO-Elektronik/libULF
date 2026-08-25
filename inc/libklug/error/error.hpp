/**
 * Error codes
 *
 * \file    inc/libklug/error/error.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cstdint>

namespace err {

enum class Error : int {
  ok = 0, ///< No error

  format = 1, ///< Response format invalid
  nak = 2,    ///< Nak
  crc = 3,    ///< Bad crc

  usb = 4, ///< Usb backend error

  unknown = 255, ///< Unknown error
};

} // namespace err
