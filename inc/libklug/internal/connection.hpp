/**
 * USB connection
 *
 * \file    inc/libklug/internal/connection.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <libusb.h>

/**
 * Connetion
 *
 * \note More of a fascade for libusb
 *
 * \todo Perhaps a simple `transmit` and `receive` would be nice
 *
 */
struct Connection {
  int open(uint16_t vid = 0x1FC9u, uint16_t pid = 0x81C1u);
  int openFd(int Fd);

  int config();
  int claim();

  int release();
  void close();

  libusb_device_handle* handle();
  uint8_t tx_ep();
  uint8_t rx_ep();
  int interface();

private:
  libusb_device_handle* _handle{nullptr}; ///< Device
  uint8_t _tx_ep, _rx_ep;                 ///< Endpoints
  int _interface;                         ///< Interface
};

/**
 * Global connection
 *
 * \deprecated Legacy before the existance of the bridge
 *
 * \todo Remove
 *
 */
inline Connection conn;
