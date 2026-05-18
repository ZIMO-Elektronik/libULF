/**
 * Transmission Base
 *
 * \file    src/libklug/internal/transmission/transmission_base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/transmission_base.hpp"
#include <libusb.h>
#include <ranges>
#include <string>
#include "libklug/internal/connection.hpp"
#include "libklug/internal/logging.hpp"

namespace transmission {

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
TransmissionBase::TransmissionBase(std::shared_ptr<Connection> conn,
                                   std::string payload,
                                   std::size_t timeout)
  : _timeout{timeout}, _conn{conn} {
  _response.reserve(64u);
  for (auto const it : payload) {
    _payload.push_back(static_cast<uint8_t>(it));
  }
}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
TransmissionBase::TransmissionBase(std::shared_ptr<Connection> conn,
                                   std::span<uint8_t const> payload,
                                   std::size_t timeout)
  : _timeout{timeout}, _conn{conn} {
  _response.reserve(64u);
  std::ranges::copy(payload, std::back_inserter(_payload));
}

/**
 * Execute transmission
 *
 * \return int 0
 *
 * \todo Refactor to return error if unsuccessful
 */
res::Result TransmissionBase::execute() {
  auto rc{this->transmit()};
  if (rc != LIBUSB_SUCCESS) return res::LibusbError{rc};

  rc = this->receive();
  if (rc != LIBUSB_SUCCESS) return res::LibusbError{rc};

  return res::Status{true};
}

/**
 * Transmit payload
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int TransmissionBase::transmit() {
  flush();

  auto rc{_conn->transmit(_payload, _timeout)};

  if (rc != 0) {
    LOGE("Transfer Error. Error: {}", libusb_error_name(rc));
    return rc;
  }

  return rc;
}

/**
 * Receive response
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS SUCCESS
 */
int TransmissionBase::receive() {
  if (_response.size() < 64u) _response.resize(64u);

  auto rc{_conn->receive(_response, _timeout)};
  if (rc != 0) {
    LOGE("Transfer Error. Error: {}", libusb_error_name(rc));
    return rc;
  }

  LOGD("Received {} Bytes", _response.size());

  return rc;
}

/**
 * Flush RX Buffer
 *
 */
void TransmissionBase::flush() {
  return;
  std::array<uint8_t, 64u> data;
  while (libusb_bulk_transfer(_conn->handle(),
                              _conn->rx_ep(),
                              std::bit_cast<unsigned char*>(data.data()),
                              data.size(),
                              nullptr,
                              1) == 0);
}

} // namespace transmission
