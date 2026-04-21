#pragma once

#include <libusb.h>

struct Connection {
  int open(uint16_t vid = 0x1FC9u, uint16_t pid = 0x81C1u);
  int openFd(int Fd);

  int config();
  int claim();

  int release();
  void close();

  libusb_device_handle *handle();
  uint8_t tx_ep();
  uint8_t rx_ep();
  int interface();

private:
  libusb_device_handle *_handle{nullptr};
  uint8_t _tx_ep, _rx_ep;
  int _interface;
};

inline Connection conn;