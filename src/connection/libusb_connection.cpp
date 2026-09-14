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
 * LibusbConnection
 *
 * \file    src/connection/libusb_connection.cpp
 * \author  Jonas Gahlert
 * \date    21.04.2026
 */

#include "libusb_connection.hpp"
#include <cassert>
#include <cstdint>
#include <format>
#include <utility>
#include <vector>
#include <ztl/ztl.hpp>
#include "log/logger.hpp"
#include "ulf/cpp/ulf_error.hpp"

namespace internal {

/**
 * Initialize the Libusb context
 *
 * \throws ulf_error   First error occurred
 */
void LibusbConnection::init() {
#ifdef ANDROID
  // We can't search devices on Android
  libusb_set_option(NULL, LIBUSB_OPTION_WEAK_AUTHORITY);
  libusb_set_option(nullptr, LIBUSB_OPTION_NO_DEVICE_DISCOVERY);
#endif
  if (auto const rc{libusb_init(nullptr)})
    throw libulf::ulf_error(libulf::map(rc),
                            "Unable to initialize libusb connection");
}

/**
 * Open connection
 *
 * \warning DO NOT CALL THIS ON ANDROID
 *
 * \param vid VID
 * \param pid PID
 *
 * \throws ulf_error   First error occurred
 */
void LibusbConnection::open(uint16_t vid, uint16_t pid) {
  if (_handle) {
    throw libulf::ulf_error{
      libulf::Error::usb,
      "Attempted to open a new device, but a device exists already"};
    std::unreachable();
  }
  _handle = libusb_open_device_with_vid_pid(nullptr, vid, pid);
  if (!_handle) {
    throw libulf::ulf_error{libulf::Error::usb, "Unable to open device."};
    std::unreachable();
  }

  LOG_TRACE("Opened device {:04x}:{:04x}", vid, pid);

  config();
  claim();
}

/**
 * Open connection from file descriptor
 *
 * \note This exists mainly for Android, as the devices need to be opened from
 * Java / Kotlin side
 *
 * \throws ulf_error   First error occurred
 */
void LibusbConnection::openFd(int Fd) {
  if (_handle) {
    throw libulf::ulf_error{
      libulf::Error::usb,
      "Attempted to open a new device, but a device exists already"};
    std::unreachable();
  }
  auto rc{libusb_wrap_sys_device(nullptr, (intptr_t)Fd, &_handle)};
  if (rc != LIBUSB_SUCCESS) {
    throw libulf::ulf_error{libulf::map(rc), "Unable to open device."};
    std::unreachable();
  }

  LOG_TRACE("Wrapped sys device");

  config();
  claim();
}

/**
 * Config connection
 *
 * \note Configures internals
 *
 * \throws ulf_error   First error occurred
 */
void LibusbConnection::config() {
  using std::operator""sv;
  if (!_handle) {
    throw libulf::ulf_error{
      libulf::Error::usb,
      "Attempted to configure connection without an open device!"};
    std::unreachable();
  }

  // Populate endpoints
  libusb_config_descriptor* config{};
  auto rc{
    libusb_get_active_config_descriptor(libusb_get_device(_handle), &config)};
  if (rc != LIBUSB_SUCCESS) {
    throw libulf::ulf_error{libulf::map(rc),
                            "Unable to retrieve config descriptor."};
    std::unreachable();
  }

  std::vector<uint8_t> tx_eps, rx_eps;

  /// \todo Redo this section
  for (int i = 0; i < config->bNumInterfaces; i++) {
    const struct libusb_interface_descriptor* inter_desc =
      &config->interface[i].altsetting[0];

    for (int k = 0; k < inter_desc->bNumEndpoints; k++) {
      const struct libusb_endpoint_descriptor* ep = &inter_desc->endpoint[k];

      // Nur Bulk-Endpunkte beachten
      if ((ep->bmAttributes & 0x03) == LIBUSB_TRANSFER_TYPE_BULK) {
        if ((ep->bEndpointAddress & 0x80) == LIBUSB_ENDPOINT_OUT) {
          tx_eps.push_back(ep->bEndpointAddress);
          _interface = i; // Interface merken für claim_interface
        } else {
          rx_eps.push_back(ep->bEndpointAddress);
        }
      }
    }
  }
  libusb_free_config_descriptor(config);

  if (tx_eps.size() != 1) {
    throw libulf::ulf_error{libulf::Error::usb,
                            "Unexpected number of TX endpoints"};
    std::unreachable();
  }
  if (rx_eps.size() != 1) {
    throw libulf::ulf_error{libulf::Error::usb,
                            "Unexpected number of RC endpoints"};
    std::unreachable();
  }

  _tx_ep = tx_eps.front();
  _rx_ep = rx_eps.front();

  LOG_TRACE("TX-EP address: x{:02x}", _tx_ep);
  LOG_TRACE("RX-EP address: x{:02x}", _rx_ep);
}

/**
 * Claim Interface
 *
 * \throws ulf_error   First error occurred
 */
void LibusbConnection::claim() {
  using std::operator""sv;
  if (!_handle) {
    throw libulf::ulf_error{
      libulf::Error::usb,
      "Attempted to claim an interface without an open device!"};
    std::unreachable();
  }

  LOG_TRACE("TX_EP - {} RX_EP - {} Interface {}", _tx_ep, _rx_ep, _interface);

  auto rc{libusb_kernel_driver_active(_handle, _interface)};
  if (rc == 1) {
    LOG_TRACE("Kernel driver attached, attempting to detach");

    rc = libusb_detach_kernel_driver(_handle, _interface);
    if (rc != LIBUSB_SUCCESS) {
      throw libulf::ulf_error{libulf::map(rc),
                              "Unable to detach kernel driver."};
      std::unreachable();
    }
    LOG_TRACE("Detached kernel driver from interface {}", _interface);
  } else {
    LOG_TRACE("libusb returned {} on driver check", rc);
  }

  libusb_detach_kernel_driver(_handle, _interface);

  rc = libusb_claim_interface(_handle, _interface);
  if (rc != LIBUSB_SUCCESS) {
    throw libulf::ulf_error{libulf::map(rc), "Unable to claim interface {}."};
    std::unreachable();
  }

  LOG_TRACE("Successfully claimed interface {}", _interface);
}

/**
 * Release Interface
 *
 * \throws ulf_error   First error occurred
 */
void LibusbConnection::release() {
  using std::operator""sv;
  if (!_handle) {
    throw libulf::ulf_error{
      libulf::Error::usb,
      "Attempted to release an interface without an open device!"};
    std::unreachable();
  }

  auto rc{libusb_release_interface(_handle, _interface)};
  if (rc != LIBUSB_SUCCESS) {
    throw libulf::ulf_error{libulf::map(rc), "Unable to release interface."};
    std::unreachable();
  }
}

/**
 * Close device
 *
 * \warning On Android, the device should be opened and closed from Java /
 * Kotlin
 */
void LibusbConnection::close() {
  release();

  if (!_handle) {
    LOG_WARN("Attemted to close a device without an open device!");
    return;
  }

  libusb_close(_handle);
  _handle = nullptr;
  return;
}

/**
 * Flush RX Buffer
 *
 * \todo Refactor, as waiting for an exception is probably not the most
 * elegant thing here
 */
void LibusbConnection::flush() {
  ztl::inplace_vector<uint8_t, 64u> buffer;
  try {
    while (true) read_all(buffer, 1);

  } catch (libulf::ulf_error e) {}
}

/**
 * Write payload
 *
 * \param payload Payload
 * \param timeout Timeout
 *
\throws ulf_error   On timeout or other error
 */
void LibusbConnection::_write(std::span<uint8_t const> payload,
                              uint32_t timeout) {
  using std::operator""sv;
  if (auto rc{
        libusb_bulk_transfer(_handle,
                             _tx_ep,
                             std::bit_cast<unsigned char*>(payload.data()),
                             payload.size(),
                             nullptr,
                             timeout)};
      rc != LIBUSB_SUCCESS)
    throw libulf::ulf_error{libulf::map(rc), "Transmit Error"};
}

/**
 * Receive to buffer
 *
 * \param buffer      Buffer
 * \param length      Buffer length
 * \param received    Actual received
 * \param terminator  Terminator
 * \param timeout     Timeout
 *
\throws ulf_error   On timeout or other error
 */
void LibusbConnection::_read_until(uint8_t* buffer,
                                   uint32_t length,
                                   int* received,
                                   uint8_t terminator,
                                   uint32_t timeout) {
  using std::operator""sv;
  if (auto rc{libusb_bulk_transfer(
        _handle, _rx_ep, buffer, length, received, timeout)};
      rc != LIBUSB_SUCCESS)
    throw libulf::ulf_error{libulf::map(rc), "Receive Error"};

} // namespace internal

/**
 * Receive to buffer
 *
 * \param buffer    Buffer
 * \param length    Buffer length
 * \param received  Actual received
 * \param timeout   Timeout
 *
\throws ulf_error   On timeout or other error
 */
void LibusbConnection::_read_all(uint8_t* buffer,
                                 uint32_t length,
                                 int* received,
                                 uint32_t timeout) {
  using std::operator""sv;
  if (auto rc{libusb_bulk_transfer(
        _handle, _rx_ep, buffer, length, received, timeout)};
      rc != LIBUSB_SUCCESS)
    throw libulf::ulf_error{libulf::map(rc), "Receive Error"};
}

} // namespace internal
