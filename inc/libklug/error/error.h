/**
 * Error codes C wrapper
 *
 * \file    inc/libklug/error/error.h
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#define LIBKLUG_ERROR_NO 0x00

#define LIBKLUG_ERROR_FORMAT 0x01

typedef enum libklug_error_t {
  ok = 0, ///< No Error

  format = 1, ///< Protocol format invalid
  nak = 2,    ///< Nak
  crc = 3,    ///< Bad crc

  usb = 4, ///< Usb backend error

  unknown = 255,
} libklug_error;
