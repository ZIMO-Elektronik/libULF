/**
 * Error 2 string C wrapper
 *
 * \file    inc/libklug/error/error2string.h
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

char const* libklug_error_2_string(uint8_t e);

#ifdef __cplusplus
}
#endif
