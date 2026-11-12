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

#include "zpp_decoder_id.hpp"
#include <decup/decup.hpp>
#include <utility>
#include "base.hpp"
#include "config.hpp"
#include "ulf/cpp/ulf_error.hpp"

namespace transmission::decup_ein {

/**
 * CTor
 *
 * \param conn USB Connection
 */
ZppDecoderId::ZppDecoderId(std::shared_ptr<internal::IConnection> conn)
  : _conn{conn} {}

/**
 * Executes the transmission
 *
 * \note
 * Executes 8 seperate transmissions and combines the result
 *
 */
void ZppDecoderId::execute() {
  Base initial_t{
    _conn,
    0u,
    static_cast<uint8_t>(decup::Command::ReadDecoderType),
  };
  initial_t.execute();
  _value |= initial_t.evaluateBool() ? 1u << 0u : 0u;

  Base t{_conn, internal::config::timeout::decup_ein::cv_read, 0xFFu};
  for (unsigned int i{1u}; i < 8u; i++) {
    t.execute();
    _value |= initial_t.evaluateBool() ? 0u : 1u << i;
  }
}

/// Stub
std::string ZppDecoderId::evaluateString() {
  using std::operator""sv;
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/// Stub
bool ZppDecoderId::evaluateBool() {
  using std::operator""sv;
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/// Returns the read value
int ZppDecoderId::evaluateValue() { return _value; }

} // namespace transmission::decup_ein
