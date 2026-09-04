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
 * MDU_EIN cv read transmission
 *
 * \file    src/transmission/mdu_ein/cv_read.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "base.hpp"
#include "transmission/i_transmission.hpp"

namespace transmission::mdu_ein {

/**
 * MDU_EIN cv read transmission
 *
 * \note Since only single bits can be read using MDU, this encapsulates 8
 * consecutive transmissions
 *
 * \internal Overrides \ref CvRead::execute to perform all transmissions
 * necessary and \ref CvRead::evaluate to provide the resulting Cv
 *
 */
struct CvRead : public ITransmission {
  CvRead(std::shared_ptr<internal::IConnection> conn, uint16_t cv);
  virtual ~CvRead() = default;

  virtual void execute() final;

  virtual std::string evaluateString() final;
  virtual bool evaluateBool() final;
  virtual int evaluateValue() final;

private:
  std::shared_ptr<internal::IConnection> _conn; ///< Connection
  uint16_t _cv;                                 ///< Cv to read
  uint8_t _value;                               ///< Result
};

} // namespace transmission::mdu_ein
