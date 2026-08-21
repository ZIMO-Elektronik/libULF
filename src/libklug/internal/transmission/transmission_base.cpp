/**
 * Transmission Base
 *
 * \file    src/libklug/internal/transmission/transmission_base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/transmission_base.hpp"
#include <ranges>
#include <string>
#include "libklug/internal/connection/i_connection.hpp"
#include "libklug/internal/logging.hpp"

namespace transmission {

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
TransmissionBase::TransmissionBase(std::shared_ptr<internal::IConnection> conn,
                                   std::string_view payload,
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
TransmissionBase::TransmissionBase(std::shared_ptr<internal::IConnection> conn,
                                   std::span<uint8_t const> payload,
                                   std::size_t timeout)
  : _timeout{timeout}, _conn{conn} {
  _response.reserve(64u);
  std::ranges::copy(payload, std::back_inserter(_payload));
}

/**
 * CTor
 *
 * \param conn        Connection
 * \param payload     Payload
 * \param terminator  Terminator
 * \param timeout     Timeout
 */
TransmissionBase::TransmissionBase(std::shared_ptr<internal::IConnection> conn,
                                   std::string_view payload,
                                   uint8_t terminator,
                                   std::size_t timeout)
  : _timeout{timeout}, _terminator{terminator}, _conn{conn} {
  _response.reserve(64u);
  for (auto const it : payload) {
    _payload.push_back(static_cast<uint8_t>(it));
  }
}

/**
 * CTor
 *
 * \param conn        Connection
 * \param payload     Payload
 * \param terminator  Terminator
 * \param timeout     Timeout
 */
TransmissionBase::TransmissionBase(std::shared_ptr<internal::IConnection> conn,
                                   std::span<uint8_t const> payload,
                                   uint8_t terminator,
                                   std::size_t timeout)
  : _timeout{timeout}, _terminator{terminator}, _conn{conn} {
  _response.reserve(64u);
  std::ranges::copy(payload, std::back_inserter(_payload));
}

/**
 * Execute transmission
 *
 * \throw libusb_error
 */
void TransmissionBase::execute() {
  this->transmit();
  this->receive();
}

/**
 * Transmit payload
 *
 * \throw libusb_error
 */
void TransmissionBase::transmit() {
  _conn->flush();
  _conn->write(_payload, _timeout);
}

/**
 * Receive response
 *
 * \throw libusb_error
 */
void TransmissionBase::receive() {
  if (_response.size() < 64u) _response.resize(64u);
  if (_terminator) _conn->read_until(_response, (*_terminator), _timeout);
  else _conn->read_all(_response, _timeout);
}

std::expected<std::string, err::Error> TransmissionBase::evaluateString() {
  return {};
}
std::expected<bool, err::Error> TransmissionBase::evaluateBool() { return {}; }
std::expected<uint8_t, err::Error> TransmissionBase::evaluateByte() {
  return {};
}

} // namespace transmission
