/**
 * Error 2 string
 *
 * \file    inc/libklug/error/error2string.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <array>
#include <string_view>
#include "error.hpp"

namespace err {

constexpr std::string_view error2string(Error e) {
  using std::operator""sv;
  switch (e) {
    case Error::ok: return "No Error"sv;
    case Error::format: return "Format Error"sv;
    case Error::nak: return "Nak Received"sv;
    case Error::crc: return "Bad CRC"sv;
    case Error::usb: return "USB Backend error"sv;
#ifdef USE_LIBSERIALPORT
    case Error::libserialport_ok: return "SP_OK"sv;
    case Error::libserialport_arg: return "SP_ERR_ARG"sv;
    case Error::libserialport_fail: return "SP_ERR_FAIL"sv;
    case Error::libserialport_mem: return "SP_ERR_MEM"sv;
    case Error::libserialport_supp: return "SP_ERR_SUPP"sv;
#elifdef USE_LIBUSB
    case Error::libusb_success: return "LIBUSB_SUCCESS"sv;
    case Error::libusb_io: return "LIBUSB_ERROR_IO"sv;
    case Error::libusb_invalid_param: return "LIBUSB_ERROR_INVALID_PARAM"sv;
    case Error::libusb_error_access: return "LIBUSB_ERROR_ACCESS"sv;
    case Error::libusb_no_device: return "LIBUSB_ERROR_NO_DEVICE"sv;
    case Error::libusb_not_found: return "LIBUSB_ERROR_NOT_FOUND"sv;
    case Error::libusb_busy: return "LIBUSB_ERROR_BUSY"sv;
    case Error::libusb_timeout: return "LIBUSB_ERROR_TIMEOUT"sv;
    case Error::libusb_overflow: return "LIBUSB_ERROR_OVERFLOW"sv;
    case Error::libusb_pipe: return "LIBUSB_ERROR_PIPE"sv;
    case Error::libusb_interrupted: return "LIBUSB_ERROR_INTERRUPTED"sv;
    case Error::libusb_no_mem: return "LIBUSB_ERROR_NO_MEM"sv;
    case Error::libusb_not_supported: return "LIBUSB_ERROR_NOT_SUPPORTED"sv;
    case Error::libusb_other: return "LIBUSB_ERROR_OTHER"sv;
#endif
    default: return "Non-listed or unknown Error"sv;
  }
}

} // namespace err
