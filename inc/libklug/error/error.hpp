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

enum class Error : uint8_t {
  none = 0x00, ///< No error

  format = 0x01, ///< Response format invalid

  nak = 0x02, ///< Nak

  crc = 0x03, ///< Bad crc
};

} // namespace err
