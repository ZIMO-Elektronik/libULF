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
 * DECUP_EIN preamble transmission
 *
 * \file    src/transmission/decup_ein/preamble.cpp
 * \author  Jonas Gahlert
 * \date    09.10.2026
 */

#include "preamble.hpp"
#include <chrono>
#include <thread>
#include "config.hpp"
#include "log/logger.hpp"

namespace transmission::decup_ein {

/**
 * Transmits the given preamble bytes 20 times in order, with a 1ms delay in
 * between.
 *
 */
void Preamble::execute() {
  if (_payload.empty()) {
    LOG_WARN("No preamble given");
    return;
  }

  for (int i{0}; i < 40 * _payload.size(); i++) {
    _conn->write(std::span<uint8_t>{_payload}.subspan(i % _payload.size(), 1u),
                 internal::config::timeout::transmit);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

} // namespace transmission::decup_ein
