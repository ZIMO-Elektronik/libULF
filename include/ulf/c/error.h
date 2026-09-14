/**
 * Copyright (C) 2026 [ZIMO Elektronik]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 *
 *
 *
 *
 *
 * libULF error
 *
 * \file    include/ulf/c/error.h
 * \author  Jonas Gahlert
 * \date    04.09.2026
 */

#ifndef ERROR_H
#define ERROR_H

#ifdef USE_LIBSERIALPORT
#  include <libserialport.h>
#elifdef USE_LIBUSB
#  include <libusb.h>
#endif

typedef enum libulf_error_t {
  LIBULF_OK = 0, ///< No error

  LIBULF_ERR_FORMAT = -1, ///< Response format invalid
  LIBULF_ERR_NAK = -2,    ///< Nak
  LIBULF_ERR_CRC = -3,    ///< Bad crc
  LIBULF_ERR_USB = -4,    ///< Usb backend error

  LIBULF_ERR_UNKNOWN = -255, ///< Unknown error

#ifdef USE_LIBSERIALPORT
  LIBULF_ERR_LIBSERIALPORT_OK = 1000 + SP_OK,
  LIBULF_ERR_LIBSERIALPORT_ARG = 1000 + SP_ERR_ARG,
  LIBULF_ERR_LIBSERIALPORT_FAIL = 1000 + SP_ERR_FAIL,
  LIBULF_ERR_LIBSERIALPORT_MEM = 1000 + SP_ERR_MEM,
  LIBULF_ERR_LIBSERIALPORT_SUPP = 1000 + SP_ERR_SUPP,
#elifdef USE_LIBUSB
  LIBULF_ERR_LIBUSB_SUCCESSerr_libusb_success = 1000 + LIBUSB_SUCCESS,
  LIBULF_ERR_LIBUSB_IO = 1000 + LIBUSB_ERROR_IO,
  LIBULF_ERR_LIBUSB_INVALID_PARAM = 1000 + LIBUSB_ERROR_INVALID_PARAM,
  LIBULF_ERR_LIBUSB_ACCESS = 1000 + LIBUSB_ERROR_ACCESS,
  LIBULF_ERR_LIBUSB_NO_DEVICE = 1000 + LIBUSB_ERROR_NO_DEVICE,
  LIBULF_ERR_LIBUSB_NOT_FOUND = 1000 + LIBUSB_ERROR_NOT_FOUND,
  LIBULF_ERR_LIBUSB_BUSY = 1000 + LIBUSB_ERROR_BUSY,
  LIBULF_ERR_LIBUSB_TIMEOUT = 1000 + LIBUSB_ERROR_TIMEOUT,
  LIBULF_ERR_LIBUSB_OVERFLOW = 1000 + LIBUSB_ERROR_OVERFLOW,
  LIBULF_ERR_LIBUSB_PIPE = 1000 + LIBUSB_ERROR_PIPE,
  LIBULF_ERR_LIBUSB_INTERRUPTED = 1000 + LIBUSB_ERROR_INTERRUPTED,
  LIBULF_ERR_LIBUSB_NO_MEM = 1000 + LIBUSB_ERROR_NO_MEM,
  LIBULF_ERR_LIBUSB_NOT_SUPPORTED = 1000 + LIBUSB_ERROR_NOT_SUPPORTED,
  LIBULF_ERR_LIBUSB_OTHER = 1000 + LIBUSB_ERROR_OTHER,
#endif
} libulf_error;

#endif
