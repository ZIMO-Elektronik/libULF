/**
 * Error codes
 *
 * \file    inc/libklug/error/error.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cstdint>

#ifdef USE_LIBSERIALPORT
#  include <libserialport.h>
#elifdef USE_LIBUSB
#  include <libusb.h>
#endif

namespace err {

enum class Error : int {
  ok = 0, ///< No error

  format = -1, ///< Response format invalid
  nak = -2,    ///< Nak
  crc = -3,    ///< Bad crc
  usb = -4,    ///< Usb backend error

  unknown = -255, ///< Unknown error

#ifdef USE_LIBSERIALPORT
  libserialport_ok = 1000 + SP_OK,
  libserialport_arg = 1000 + SP_ERR_ARG,
  libserialport_fail = 1000 + SP_ERR_FAIL,
  libserialport_mem = 1000 + SP_ERR_MEM,
  libserialport_supp = 1000 + SP_ERR_SUPP,
#elifdef USE_LIBUSB
  libusb_success = 1000 + LIBUSB_SUCCESS,
  libusb_io = 1000 + LIBUSB_ERROR_IO,
  libusb_invalid_param = 1000 + LIBUSB_ERROR_INVALID_PARAM,
  libusb_error_access = 1000 + LIBUSB_ERROR_ACCESS,
  libusb_no_device = 1000 + LIBUSB_ERROR_NO_DEVICE,
  libusb_not_found = 1000 + LIBUSB_ERROR_NOT_FOUND,
  libusb_busy = 1000 + LIBUSB_ERROR_BUSY,
  libusb_timeout = 1000 + LIBUSB_ERROR_TIMEOUT,
  libusb_overflow = 1000 + LIBUSB_ERROR_OVERFLOW,
  libusb_pipe = 1000 + LIBUSB_ERROR_PIPE,
  libusb_interrupted = 1000 + LIBUSB_ERROR_INTERRUPTED,
  libusb_no_mem = 1000 + LIBUSB_ERROR_NO_MEM,
  libusb_not_supported = 1000 + LIBUSB_ERROR_NOT_SUPPORTED,
  libusb_other = 1000 + LIBUSB_ERROR_OTHER,
#endif
};

#ifdef USE_LIBSERIALPORT
constexpr Error map(int r) {
  return static_cast<Error>(static_cast<int>(r) + 1000);
}
#endif

#ifdef USE_LIBUSB
constexpr Error map(int r) {
  return static_cast<Error>(static_cast<int>(r) + 1000);
}
#endif

} // namespace err
