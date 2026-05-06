/**
 * Error 2 String C wrapper
 *
 * \file    src/libklug/error/error2string.cpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#include "libklug/error/error2string.h"
#include "libklug/error/error2string.hpp"

/**
 * Error 2 String C wrapper
 *
 * \param e
 * \return char const*
 */
char const* libklug_error_2_string(uint8_t e) {
  return error::error2string(static_cast<error::Error>(e)).data();
}
