/**
 * Result
 *
 * \file    inc/libklug/result/result.h
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include "result_type.h"

/**
 * Result
 *
 */
typedef struct {
  result_type type;
  union {
    int success;
    int value;
    char const* string;
    int error;
    int libusb_error;
  } data;
} result;
