/**
 * Error codes
 *
 * \file    inc/libklug/error/error.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cstdint>

namespace error {

enum class Error : uint8_t {
  none = 0x00, ///< No error

  format = 0x01, ///< Response format invalid
};

} // namespace error
