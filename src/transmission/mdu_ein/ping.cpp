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
 * MDU_EIN Ping
 *
 * \file    src/transmission/mdu_ein/ping.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "ping.hpp"
#include <ulf/mdu_ein.hpp>
#include <utility>
#include "klug/cpp/klug_error.hpp"

namespace transmission::mdu_ein {

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Ping::Ping(std::shared_ptr<internal::IConnection> conn,
           std::string payload,
           std::size_t timeout)
  : Base{conn, payload, timeout} {}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Ping::Ping(std::shared_ptr<internal::IConnection> conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : Base{conn, payload, timeout} {}

/**
 * Evaluate a bool
 *
 * \note
 * This is the inverse operation, since a decoder naks when it responds
 *
 * \throw generic_error When format does not match protocol
 *
 * \retval Evaluated bool
 */
bool Ping::evaluateBool() {
  using std::operator""sv;
  if (!valid()) {
    throw libklug::klug_error{libklug::Error::format, "Format Mismatch"};
    std::unreachable();
  }
  return _response[0] == ulf::mdu_ein::ack && _response[2] == ulf::mdu_ein::nak;
}

} // namespace transmission::mdu_ein
