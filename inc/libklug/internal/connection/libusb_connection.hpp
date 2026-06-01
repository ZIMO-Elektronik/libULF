/**
 * USB connection
 *
 * \file    inc/libklug/internal/connection/libusb_connection.hpp
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
  virtual int init() override;

  virtual int open(uint16_t vid = 0x1FC9u, uint16_t pid = 0x81C1u);
  virtual int openFd(int Fd);

  virtual int config();
  virtual int claim();

  virtual int release();
  virtual void close();

  virtual void flush();

private:
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
