/**
 * Copyright (C) 2026 ZIMO Elektronik
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
 * USB connection
 *
 * \file    src/connection/libusb_connection.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <libusb.h>
#include <ranges>
#include "i_connection.hpp"

namespace internal {

/**
 * Connetion
 *
 * \details
 * A connection class using libusb as backend
 *
 */
struct LibusbConnection : IConnection {
  virtual void init() override;

  virtual void open(uint16_t vid = 0x1FC9u, uint16_t pid = 0x81C1u) override;
  virtual void openFd(int Fd) override;

  virtual void close() override;

  virtual void flush() override;

private:
  void config();
  void claim();

  void release();

  virtual void _write(std::span<uint8_t const> payload,
                      uint32_t timeout) override;
  virtual void _read_until(uint8_t* buffer,
                           uint32_t length,
                           int* received,
                           uint8_t terminator,
                           uint32_t timeout) override;
  virtual void _read_all(uint8_t* buffer,
                         uint32_t length,
                         int* received,
                         uint32_t timeout) override;

  libusb_device_handle* _handle{nullptr}; ///< Device
  uint8_t _tx_ep, _rx_ep;                 ///< Endpoints
  int _interface;                         ///< Interface
};

} // namespace internal
