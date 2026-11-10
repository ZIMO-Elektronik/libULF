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
 * \file    src/transmission/decup_ein/cv_read.hpp
 * \author  Jonas Gahlert
 * \date    10.11.2026
 */

#pragma once

#include "base.hpp"
#include "connection/i_connection.hpp"
#include "transmission/i_transmission.hpp"

namespace transmission::decup_ein {

/**
 * DECUP_EIN zpp decoder id get transmission
 *
 * \details
 * This struct combines all necessary transmissions into one execution, while
 * combining their result.
 *
 */
struct ZppDecoderId : ITransmission {
  ZppDecoderId(std::shared_ptr<internal::IConnection> conn);
  virtual ~ZppDecoderId() = default;

  virtual void execute() final;

  virtual std::string evaluateString() final;
  virtual bool evaluateBool() final;
  virtual int evaluateValue() final;

private:
  std::shared_ptr<internal::IConnection> _conn; ///< USB Connection

  uint8_t _value{}; ///< Value
};

} // namespace transmission::decup_ein
