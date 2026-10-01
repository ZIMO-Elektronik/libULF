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
 * MDU_EIN base transmission
 *
 * \file    src/transmission/mdu_ein/base.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "transmission/transmission_base.hpp"

namespace transmission::mdu_ein {

/**
 * MDU_EIN base transmission
 *
 * \internal Overrides \ref Base::evaluate
 *
 */
struct Base : public TransmissionBase {
  Base(std::shared_ptr<internal::IConnection> conn,
       std::string payload,
       std::size_t timeout);
  Base(std::shared_ptr<internal::IConnection> conn,
       std::span<uint8_t const> payload,
       std::size_t timeout);
  virtual ~Base() = default;

  virtual bool evaluateBool() override;

protected:
  bool valid();
};

} // namespace transmission::mdu_ein
