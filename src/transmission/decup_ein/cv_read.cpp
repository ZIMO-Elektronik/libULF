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
 * DECUP_EIN cv read transmission
 *
 * \file    src/transmission/decup_ein/cv_read.cpp
 * \author  Jonas Gahlert
 * \date    10.11.2026
 */

#include "cv_read.hpp"
#include <decup/decup.hpp>
#include <utility>
#include "base.hpp"
#include "ulf/cpp/ulf_error.hpp"

namespace transmission::decup_ein {

CvRead::CvRead(std::shared_ptr<internal::IConnection> conn, uint16_t cv)
  : _conn{conn}, _cv{cv} {}

void CvRead::execute() {
  Base initial_t{_conn,
                 0u,
                 static_cast<uint8_t>(decup::Command::CvRead),
                 static_cast<uint8_t>(_cv >> 8u),
                 static_cast<uint8_t>(_cv >> 0u)};
  initial_t.execute();
  _value |= initial_t.evaluateBool() ? 1u << 0u : 0u;

  Base t{_conn, 0u, 0xFFu};
  for (unsigned int i{1u}; i < 8u; i++) {
    t.execute();
    _value |= initial_t.evaluateBool() ? 1u << i : 0u;
  }
}

/// Stub
std::string CvRead::evaluateString() {
  using std::operator""sv;
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/// Stub
bool CvRead::evaluateBool() {
  using std::operator""sv;
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

int CvRead::evaluateValue() { return _value; }

} // namespace transmission::decup_ein
