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
 * MDU_EIN Cv Read
 *
 * \file    src/transmission/mdu_ein/cv_read.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "cv_read.hpp"
#include <format>
#include <ulf/mdu_ein.hpp>
#include <utility>
#include "config.hpp"
#include "klug/cpp/klug_error.hpp"
#include "transmission/mdu_ein/base.hpp"

namespace transmission::mdu_ein {

/**
 * CTor
 *
 * \param conn  Connection
 * \param cv    Cv address
 */
CvRead::CvRead(std::shared_ptr<internal::IConnection> conn, uint16_t cv)
  : _conn{conn}, _cv{cv}, _value{0u} {}

/**
 * Execute
 *
 * \throw libklug_error   If format does not match protocol or the device is
 *                        unresponsive
 *
 * \todo Maybe retry single bits
 */
void CvRead::execute() {
  for (uint8_t i{0}; i < sizeof(_value) * 8u; i++) {
    Base t{_conn,
           ulf::mdu_ein::bytes2mdu_ein(mdu::make_cv_read_packet(_cv, i)),
           internal::config::timeout::mdu_ein::cv_read};
    t.execute();
    _value |= !(t.evaluateBool()) << i;
  }
}

/// Stub
std::string CvRead::evaluateString() {
  using std::operator""sv;
  throw libklug::klug_error{libklug::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/// Stub
bool CvRead::evaluateBool() {
  using std::operator""sv;
  throw libklug::klug_error{libklug::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/**
 * Return value
 *
 * \return uint8_t
 */
int CvRead::evaluateValue() { return static_cast<int>(_value); }

} // namespace transmission::mdu_ein
