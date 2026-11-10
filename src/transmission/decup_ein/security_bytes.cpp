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
 * DECUP_EIN security bytes transmission
 *
 * \file    src/transmission/decup_ein/security_bytes.cpp
 * \author  Jonas Gahlert
 * \date    10.11.2026
 */

#include "security_bytes.hpp"
#include <utility>
#include "ulf/cpp/ulf_error.hpp"

namespace transmission::decup_ein {

SecurityBytes::SecurityBytes(std::shared_ptr<internal::IConnection> conn,
                             std::size_t timeout)
  : _conn{conn}, _timeout{timeout} {}

void SecurityBytes::execute() {
  auto const l{[this](uint8_t const byte) {
    Base t{_conn, _timeout, byte};
    t.execute();
    return t.evaluateBool();
  }};

  _first_success = l(0x55u);
  _second_success = l(0xAAu);
}

/// Stub
std::string SecurityBytes::evaluateString() {
  using std::operator""sv;
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

bool SecurityBytes::evaluateBool() { return _first_success && _second_success; }

/// Stub
int SecurityBytes::evaluateValue() {
  using std::operator""sv;
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

} // namespace transmission::decup_ein
