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
 * Transmission Base
 *
 * \file    src/transmission/transmission_base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "transmission_base.hpp"
#include <ranges>
#include <string>
#include <utility>
#include "connection/i_connection.hpp"
#include "klug/cpp/klug_error.hpp"

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
 * \throw libklug_error   If the device is unresponsive
 */
void TransmissionBase::execute() {
  this->transmit();
  this->receive();
}

/**
 * Transmit payload
 *
 * \throw libklug_error   If the device is unresponsive
 */
void TransmissionBase::transmit() {
  _conn->flush();
  _conn->write(_payload, _timeout);
}

/**
 * Receive response
 *
 * \throw libklug_error   If the device is unresponsive
 */
void TransmissionBase::receive() {
  if (_response.size() < 64u) _response.resize(64u);
  if (_terminator) _conn->read_until(_response, (*_terminator), _timeout);
  else _conn->read_all(_response, _timeout);
}

/// Stub
std::string TransmissionBase::evaluateString() {
  throw libklug::klug_error{libklug::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/// Stub
bool TransmissionBase::evaluateBool() {
  throw libklug::klug_error{libklug::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/// Stub
int TransmissionBase::evaluateValue() {
  throw libklug::klug_error{libklug::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

} // namespace transmission
