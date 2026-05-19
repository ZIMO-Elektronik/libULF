/**
 * Result
 *
 * \file    inc/libklug/result/result.h
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include "result_type.h"

#define LIBKLUG_SUCCESS 1
#define LIBKLUG_BUSY 0

#define LIBKLUG_TRUE 1
#define LIBKLUG_FALSE 0

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
