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
#include "libklug/internal/log/asserter.hpp"
#include "libklug/internal/log/logger.hpp"
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
  LOG_INFO("Attempt to open device with [{:04x}:{:04x}]", pid, vid);
  sp_port** port_list;
  sp_port* found_port = nullptr;

  if (sp_list_ports(&port_list) != SP_OK) {
    LOG_ERROR("Unable to list ports SP_ERR[{}]", sp_last_error_code());
    return -1; // Error while listing
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
    LOG_ERROR("No ZIMO_Interface found");
    return -2; // Gerät nicht gefunden
  }

  this->_port = found_port;
  if (sp_open(_port, SP_MODE_READ_WRITE) != SP_OK) {
    LOG_ERROR("Unable to open device on port {}", sp_get_port_name(found_port));
    return -3;
  }

  LOG_INFO("Device on port {} now open", sp_get_port_name(found_port));
  return 0;
}

/**
 * Open filedescriptor
 *
 * \param Fd
 * \return int
 */
int LibserialportConnection::openFd(int Fd) {
  LOG_ERROR("openFd is an illegal op for libserialport");
  return -1;
}

int LibserialportConnection::config() {
  if (!_port) {
    LOG_ERROR("Attempted to configure port=NULL");
    return -1;
  }

  for (unsigned int i{0uz}; i < 5uz; i++) {
    auto rc{SP_OK};
    switch (i) {
      case 0: rc = sp_set_baudrate(_port, 115200); break;
      case 1: rc = sp_set_bits(_port, 8); break;
      case 2: rc = sp_set_parity(_port, SP_PARITY_NONE); break;
      case 3: rc = sp_set_stopbits(_port, 1); break;
      case 4: rc = sp_set_flowcontrol(_port, SP_FLOWCONTROL_NONE); break;
      default: LIBKLUG_ASSERT(false) << "Config counter out of bounds\n"; break;
    }
    if (rc != SP_OK) {
      LOG_ERROR("Unable to configure device on port {} SP_ERR[{}]",
                sp_get_port_name(_port),
                sp_last_error_code());
      return -2;
    }
  }

  LOG_INFO("Configured device on port {}", sp_get_port_name(_port));
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

void LibserialportConnection::_write(std::span<uint8_t const> payload,
                                     uint32_t timeout) {
  using std::operator""sv;
  assert(_port);

  LOG_TRACE(
    "Attempting to transmit. Timeout: {}, Payload {:x}", timeout, payload);
  auto r{sp_blocking_write(_port, payload.data(), payload.size(), timeout)};
  if (r < 0) {
    auto sp_err{sp_last_error_message()};
    std::string err{"Transmit Error: " + std::to_string(r) + " - " + sp_err};
    sp_free_error_message(sp_err);
    throw except::generic_error{err::Error::usb, err};
  }
  LOG_TRACE("Successfully transmitted {} bytes", std::to_underlying(r));
}

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
      throw except::generic_error{
        err::Error::usb,
        std::string{"Receive Error: SP_ERR[" +
                    std::to_string(sp_last_error_code()) + "]"}};
    }

    if (buffer[_received++] == terminator) break;
  }

  LOG_TRACE("Successfully received {} bytes. Payload {:x}",
            _received,
            std::span<uint8_t const>(buffer, _received));
  *received = _received;
}

void LibserialportConnection::_read_all(uint8_t* buffer,
                                        uint32_t length,
                                        int* received,
                                        uint32_t timeout) {
  assert(_port);
  LOG_TRACE("Attempting to receive. Timeout: {}", timeout);
  auto r{sp_blocking_read_next(_port, buffer, length, timeout)};
  if (r < 0) {
    auto sp_err{sp_last_error_message()};
    std::string err{"Receive Error: " + std::to_string(r) + " - " + sp_err};
    sp_free_error_message(sp_err);
    throw except::generic_error{err::Error::usb, err};
  }
  LOG_TRACE("Successfully received {} bytes. Payload {:x}",
            std::to_underlying(r),
            std::span<uint8_t const>(buffer, r));
  *received = r;
}

} // namespace internal
