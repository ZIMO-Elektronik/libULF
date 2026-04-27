#include "new_port.hpp"

#include <cassert>

#include <iostream>
#include <vector>

#include "internal/connection.hpp"
#include "internal/logging.hpp"
#include "internal/transmission.hpp"

std::array<unsigned char, 64> in_data;

int init() {
#ifdef ANDROID
  // We cant search devices on Android
  libusb_set_option(NULL, LIBUSB_OPTION_WEAK_AUTHORITY);
  libusb_set_option(nullptr, LIBUSB_OPTION_NO_DEVICE_DISCOVERY);
#endif
  return libusb_init(NULL);
}

bool open_klug() {
  auto rc{conn.open()};
  if (rc != LIBUSB_SUCCESS) return false;

  rc = conn.config();
  if (rc != LIBUSB_SUCCESS) return false;

  return conn.claim() == LIBUSB_SUCCESS;
}

int openWithFd(int Fd) {
  auto rc{conn.openFd(Fd)};
  if (rc != LIBUSB_SUCCESS) return rc;

  rc = conn.config();
  if (rc != LIBUSB_SUCCESS) return rc;

  return conn.claim();
}

bool close_klug() {
  if (!conn.release()) return false;
  conn.close();
  return true;
}

char const* ping_klug() {
  transmission::Transmission transmission(conn, "PING\r", 0);

  transmission.execute();

  auto const result{transmission.result()};

  for (int i{0}; i < result.size(); i++) { in_data[i] = result[i]; }

  LOGD("Result: {}",
       std::string_view(std::bit_cast<char const*>(in_data.data()),
                        result.size()));

  return std::bit_cast<char const*>(in_data.data());
}