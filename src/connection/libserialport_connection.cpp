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
 * LibserialportConnection
 *
 * \file    src/connection/libserialport_connection.cpp
 * \author  Jonas Gahlert
 * \date    29.05.2026
 */

#include "libserialport_connection.hpp"
#include <libserialport.h>
#include <cassert>
#include <string_view>
#include <utility>
#include "log/asserter.hpp"
#include "log/logger.hpp"
#include "ulf/cpp/error.hpp"
#include "ulf/cpp/ulf_error.hpp"

namespace internal {

/**
 * Init (fake)
 *
 * \return int 0
 */
void LibserialportConnection::init() {}

/**
 * Open device
 *
 * \param vid Device VID
 * \param pid Device PID
 *
 * \throws ulf_error   First error occurred
 */
void LibserialportConnection::open(uint16_t vid, uint16_t pid) {
  using std::operator""sv;
  LOG_INFO("Attempt to open device with [{:04x}:{:04x}]", pid, vid);
  sp_port** port_list;
  sp_port* found_port = nullptr;

  if (sp_list_ports(&port_list) != SP_OK) {
    throw libulf::ulf_error{libulf::map(sp_last_error_code()),
                            "Unable to retrieve port list"};
    std::unreachable();
  }

  for (int i = 0; port_list[i] != nullptr; i++) {
    if (sp_get_port_transport(port_list[i]) == SP_TRANSPORT_USB) {
      int vid_ = 0, pid_ = 0;
      sp_get_port_usb_vid_pid(port_list[i], &vid_, &pid_);
      if (vid_ == vid && pid_ == pid) {
        sp_copy_port(port_list[i], &found_port);
        LOG_INFO("Found ZIMO_Interface on port {}",
                 sp_get_port_name(found_port));
        break;
      }
    }
  }
  sp_free_port_list(port_list);

  if (!found_port) {
    throw libulf::ulf_error{libulf::Error::usb, "No ZIMO_Interface found"};
    std::unreachable();
  }

  this->_port = found_port;
  if (sp_open(_port, SP_MODE_READ_WRITE) != SP_OK) {
    throw libulf::ulf_error{
      libulf::map(sp_last_error_code()),
      "Unable to open device",
    };
    std::unreachable();
  }

  LOG_INFO("Device on port {} now open", sp_get_port_name(found_port));
}

/**
 * Open filedescriptor
 *
 * \param Fd
 *
 * \throws ulf_error   First error occurred
 */
void LibserialportConnection::openFd(int Fd) {
  using std::operator""sv;
  throw libulf::ulf_error{
    libulf::Error::usb,
    "Libserialport is unable to open a device by FileDescriptor"};
  std::unreachable();
}

/**
 * Configure device
 *
 * \throws ulf_error   First error occurred
 */
void LibserialportConnection::config() {
  using std::operator""sv;
  if (!_port) {
    throw libulf::ulf_error{libulf::Error::usb,
                            "Attempted to configure port=NULL"};
    std::unreachable();
  }

  for (unsigned int i{0uz}; i < 5uz; i++) {
    auto rc{SP_OK};
    switch (i) {
      case 0: rc = sp_set_baudrate(_port, 115200); break;
      case 1: rc = sp_set_bits(_port, 8); break;
      case 2: rc = sp_set_parity(_port, SP_PARITY_NONE); break;
      case 3: rc = sp_set_stopbits(_port, 1); break;
      case 4: rc = sp_set_flowcontrol(_port, SP_FLOWCONTROL_NONE); break;
      default: LIBULF_ASSERT(false) << "Config counter out of bounds\n"; break;
    }
    if (rc != SP_OK) {
      throw libulf::ulf_error{libulf::map(rc), "Unable to configure device"};
      std::unreachable();
    }
  }

  LOG_INFO("Configured device on port {}", sp_get_port_name(_port));
}

/**
 * Close device
 *
 */
void LibserialportConnection::close() {
  if (_port) {
    sp_flush(_port, SP_BUF_BOTH);
    sp_close(_port);
    sp_free_port(_port);
    _port = nullptr;
  }
}

/**
 * Flush device buffers
 *
 */
void LibserialportConnection::flush() {
  if (_port) sp_flush(_port, SP_BUF_BOTH);
}

/**
 * Write payload to out buffer
 *
 * \param payload Payload
 * \param timeout Timeout
 *
 * \throws ulf_error   On timeout or other error
 */
void LibserialportConnection::_write(std::span<uint8_t const> payload,
                                     uint32_t timeout) {
  using std::operator""sv;
  assert(_port);

  LOG_TRACE(
    "Attempting to transmit. Timeout: {}, Payload {:x}", timeout, payload);
  auto r{sp_blocking_write(_port, payload.data(), payload.size(), timeout)};
  if (r < 0) {
    throw libulf::ulf_error{libulf::map(sp_last_error_code()),
                            "Transmit error"};
    std::unreachable();
  }
  LOG_TRACE("Successfully transmitted {} bytes", std::to_underlying(r));
}

/**
 * Read until terminator symbol
 *
 * \param buffer      Buffer to read into
 * \param length      Length of buffer
 * \param received    Size of data received
 * \param terminator  Terminator symbol
 * \param timeout     Timeout
 *
 * \throws ulf_error   On timeout or other error
 */
void LibserialportConnection::_read_until(uint8_t* buffer,
                                          uint32_t length,
                                          int* received,
                                          uint8_t terminator,
                                          uint32_t timeout) {
  assert(_port);
  LOG_TRACE("Attempting to receive data untit terminator [{:02x}]. Timeout: {}",
            terminator,
            timeout);

  int _received{0};

  while (true) {
    // Receive until timeout, error, or terminator
    if (sp_blocking_read(_port, buffer + _received, 1, timeout) <= 0) {
      throw libulf::ulf_error{libulf::map(sp_last_error_code()),
                              "Receive Error"};
      std::unreachable();
    }

    if (buffer[_received++] == terminator) break;
  }

  LOG_TRACE("Successfully received {} bytes. Payload {:x}",
            _received,
            std::span<uint8_t const>(buffer, _received));
  *received = _received;
}

/**
 * Read all data from buffer
 *
 * \param buffer    Buffer to read into
 * \param length    Size of read buffer
 * \param received  Received data size
 * \param timeout   Timeout
 *
 * \throws ulf_error   On timeout or other error
 */
void LibserialportConnection::_read_all(uint8_t* buffer,
                                        uint32_t length,
                                        int* received,
                                        uint32_t timeout) {
  assert(_port);
  LOG_TRACE("Attempting to receive. Timeout: {}", timeout);
  auto r{sp_blocking_read_next(_port, buffer, length, timeout)};
  if (r < 0) {
    throw libulf::ulf_error{libulf::map(sp_last_error_code()), "Receive Error"};
    std::unreachable();
  }
  LOG_TRACE("Successfully received {} bytes. Payload {:x}",
            std::to_underlying(r),
            std::span<uint8_t const>(buffer, r));
  *received = r;
}

} // namespace internal
