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
 * SUSUV2 Base transmission
 *
 * \file    src/transmission/susiv2/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "base.hpp"
#include <ulf/susiv2.hpp>
#include <utility>
#include "ulf/cpp/ulf_error.hpp"

namespace transmission::susiv2 {

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
  : TransmissionBase{conn, payload, timeout} {}

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
  : TransmissionBase{conn, payload, timeout} {}

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
    throw libulf::ulf_error{libulf::Error::format, "Format Mismatch"};
    std::unreachable();
  }
  return _response.front() == ulf::susiv2::ack;
}

/**
 * Check if the response is valid
 *
 * \note
 * Yes, a nak is indeed valid
 *
 * \return true   Valid
 * \return false  Not valid
 */
bool Base::valid() {
  return _response.size() >= 1uz && _response.size() <= 6uz &&
         (_response.front() == ulf::susiv2::ack ||
          _response.front() == ulf::susiv2::nak);
}

} // namespace transmission::susiv2
