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

struct CvRead : ITransmission {
  CvRead(std::shared_ptr<internal::IConnection> conn, uint16_t cv);
  virtual ~CvRead() = default;

  virtual void execute() final;

  virtual std::string evaluateString() final;
  virtual bool evaluateBool() final;
  virtual int evaluateValue() final;

private:
  std::shared_ptr<internal::IConnection> _conn;

  uint16_t _cv;
  uint8_t _value{};
};

} // namespace transmission::decup_ein
