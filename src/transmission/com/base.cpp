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
 * ULF_COM Base transmission
 *
 * \file    src/transmission/com/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "base.hpp"
#include <utility>
#include "ulf/cpp/ulf_error.hpp"

std::array<char, 64> tmp_buffer;

namespace transmission::com {

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 *
 * \todo Put terminator behind constant
 */
Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::string payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, '\r', timeout} {}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 *
 * \todo Put terminator behind constant
 */
Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, '\r', timeout} {}

/**
 * Evaluate a string
 *
 * \return Received string
 */
std::string Base::evaluateString() {
  return std::string{reinterpret_cast<char const*>(_response.data()),
                     _response.size()};
}

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
  if (std::string_view{std::bit_cast<char const*>(_response.data()),
                       _response.size()} == "OK\r"sv)
    return true;
  else if (std::string_view{std::bit_cast<char const*>(_response.data()),
                            _response.size()} == "NOT_OK\r"sv)
    return false;

  throw libulf::ulf_error{libulf::Error::format, "Format mismatch"};
  std::unreachable();
}

} // namespace transmission::com
