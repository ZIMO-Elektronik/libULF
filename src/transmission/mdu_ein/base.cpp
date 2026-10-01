/**
 * Copyright (C) 2026 ZIMO Elektronik
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
 * MDU_EIN Base transmission
 *
 * \file    src/transmission/mdu_ein/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "base.hpp"
#include <ulf/mdu_ein.hpp>
#include <utility>
#include "ulf/cpp/ulf_error.hpp"

namespace transmission::mdu_ein {

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::string payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, ulf::mdu_ein::end, timeout} {}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, ulf::mdu_ein::end, timeout} {}

/**
 * Evaluate a bool
 *
 * \throw libulf_error   If format does not match protocol or the device is
 *                        unresponsive
 *
 * \return Evaluated bool
 */
bool Base::evaluateBool() {
  using std::operator""sv;
  if (!valid()) {
    throw libulf::ulf_error{libulf::Error::format, "Format mismatch"};
    std::unreachable();
  }
  return _response[0] == ulf::mdu_ein::ack && _response[2] == ulf::mdu_ein::ack;
}

/**
 * Response valid
 *
 * \return true   Valid
 * \return false  Invalid
 */
bool Base::valid() {
  return !(_response.size() != 4uz || _response[1] != ulf::mdu_ein::separator ||
           _response[3] != ulf::mdu_ein::end);
}

} // namespace transmission::mdu_ein
