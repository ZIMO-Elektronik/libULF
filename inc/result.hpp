/**
 * Result class
 *
 * \file    inc/result.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

/**
 * Result type
 *
 */
typedef enum {
  status,
  cv,
  string,
  error,
  libusb_error,
} result_type;

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
} result_t;