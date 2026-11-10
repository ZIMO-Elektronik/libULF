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
 * DECUP_EIN base transmission
 *
 * \file    src/transmission/decup_ein/base.hpp
 * \author  Jonas Gahlert
 * \date    09.10.2026
 */

#include "base.hpp"
#include <ulf/decup_ein.hpp>
#include <utility>
#include "ulf/cpp/ulf_error.hpp"

namespace transmission::decup_ein {

Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::string_view payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, timeout} {}

Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, timeout} {}

bool Base::evaluateBool() {
  using std::operator""sv;
  if (!valid()) {
    throw libulf::ulf_error{libulf::Error::format, "Format mismatch"};
    std::unreachable();
  }

  return _response.front() == ulf::decup_ein::ack;
}

bool Base::valid() {
  return _response.size() == 1u && (_response.front() == ulf::decup_ein::ack ||
                                    _response.front() == ulf::decup_ein::nak);
}

} // namespace transmission::decup_ein
