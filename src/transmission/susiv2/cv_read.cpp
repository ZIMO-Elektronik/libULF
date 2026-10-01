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
 * SUSIV2 CvRead
 *
 * \file    src/transmission/susiv2/cv_read.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "cv_read.hpp"
#include <ulf/susiv2.hpp>
#include <utility>
#include <zusi/utility.hpp>
#include "ulf/cpp/ulf_error.hpp"

namespace transmission::susiv2 {

/**
 * CTor
 *
 * \param conn    Connection
 * \param timeout Timeout
 * \param cv      Cv address
 */
CvRead::CvRead(std::shared_ptr<internal::IConnection> conn,
               size_t timeout,
               uint16_t cv)
  : Base{conn,
         ulf::susiv2::packet2frame(zusi::make_cv_read_packet(0, cv)),
         timeout} {}

/**
 * Evaluate a byte
 *
 * \return Evaluated value
 */
int CvRead::evaluateValue() {
  using std::operator""sv;
  if (!valid()) {
    throw libulf::ulf_error{libulf::Error::format, "Format Mismatch"};
    std::unreachable();
  }
  if (_response[0u] == ulf::susiv2::nak) return -1;
  return static_cast<int>(_response[1u]);
}

} // namespace transmission::susiv2
