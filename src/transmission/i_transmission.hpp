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
 * Transmission interface
 *
 * \file    src/transmission/i_transmission.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <cstdint>
#include <expected>
#include <string>

namespace transmission {

/**
 * Transmission Interface
 *
 * \details
 * Inheriting transmissions can be executed using \ref ITransmission::execute.
 * This will transmit and receive ONCE and produce a result. The result can be
 * extracted using \ref ITransmission::evaluate.
 *
 * \note
 * Per design of the ULF_COM protocol, it is recommended to have only one active
 * transmission. Multiple parallel transmissions WILL result in UB
 */
struct ITransmission {
  virtual ~ITransmission() = default;

  virtual void execute() = 0;

  virtual std::string evaluateString() = 0;
  virtual bool evaluateBool() = 0;
  virtual int evaluateValue() = 0;
};

} // namespace transmission
