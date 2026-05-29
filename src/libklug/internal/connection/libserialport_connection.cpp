/**
 * LibserialportConnection
 *
 * \file    src/libklug/internal/connection/libserialport_connection.cpp
 * \author  Jonas Gahlert
 * \date    29.05.2026
 */

#include "libklug/internal/connection/libserialport_connection.hpp"
#include <libserialport.h>
#include <cassert>
#include <string_view>
#include "libklug/error/error.hpp"
#include "libklug/internal/exception/e_generic.hpp"
#include "libklug/internal/logging.hpp"

namespace internal {

/**
 * Init (fake)
 *
 * \return int 0
 */
int LibserialportConnection::init() { return 0; }

/**
 * Open device
 *
 * \param vid Device VID
 * \param pid Device PID
 * \return int
 */
int LibserialportConnection::open(uint16_t vid, uint16_t pid) {
  sp_port** port_list;
  sp_port* found_port = nullptr;

  if (sp_list_ports(&port_list) != SP_OK) {
    return -1; // Fehler beim Auflisten
  }

  for (int i = 0; port_list[i] != nullptr; i++) {
    if (sp_get_port_transport(port_list[i]) == SP_TRANSPORT_USB) {
      int vid_ = 0, pid_ = 0;
      sp_get_port_usb_vid_pid(port_list[i], &vid_, &pid_);
      if (vid_ == vid && pid_ == pid) {
        sp_copy_port(port_list[i], &found_port);
        break;
      }
    }
  }
  sp_free_port_list(port_list);

  if (!found_port) {
    return -2; // Gerät nicht gefunden
  }

  this->_port = found_port;
  auto rc = sp_open(_port, SP_MODE_READ_WRITE);
  return 0; // Erfolg
}

/**
 * Open filedescriptor
 *
 * \param Fd
 * \return int
 */
int LibserialportConnection::openFd(int Fd) { return -1; }

int LibserialportConnection::config() {
  if (!_port) return -1;

  auto rc = sp_set_baudrate(_port, 115200);
  rc = sp_set_bits(_port, 8);
  rc = sp_set_parity(_port, SP_PARITY_NONE);
  rc = sp_set_stopbits(_port, 1);
  rc = sp_set_flowcontrol(_port, SP_FLOWCONTROL_NONE);

  return 0;
}

int LibserialportConnection::claim() {
  if (!_port) return -1;
  return 0;
}

int LibserialportConnection::release() {
  if (!_port) return -1;
  sp_flush(_port, SP_BUF_BOTH);
  return 0;
}

void LibserialportConnection::close() {
  if (_port) {
    sp_close(_port);
    sp_free_port(_port);
    _port = nullptr;
  }
}

void LibserialportConnection::flush() {
  if (_port) sp_flush(_port, SP_BUF_BOTH);
}

void LibserialportConnection::_transmit(std::span<uint8_t const> payload,
                                        uint32_t timeout) {
  using std::operator""sv;
  assert(_port);
  auto r{sp_blocking_write(_port, payload.data(), payload.size(), timeout)};
  if (r < 0) {
    auto sp_err{sp_last_error_message()};
    std::string err{"Transmit Error: " + std::to_string(r) + " - " + sp_err};
    sp_free_error_message(sp_err);
    throw except::generic_error{err::Error::usb, err};
  }
}

void LibserialportConnection::_receive(uint8_t* buffer,
                                       uint32_t length,
                                       int* received,
                                       uint32_t timeout) {
  assert(_port);
  auto r{sp_blocking_read_next(_port, buffer, length, timeout)};
  if (r < 0) {
    auto sp_err{sp_last_error_message()};
    std::string err{"Receive Error: " + std::to_string(r) + " - " + sp_err};
    sp_free_error_message(sp_err);
    throw except::generic_error{err::Error::usb, err};
  }
  *received = r;
}

} // namespace internal
