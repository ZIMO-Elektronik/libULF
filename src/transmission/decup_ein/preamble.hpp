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
 * \file    src/transmission/decup_ein/preamble.hpp
 * \author  Jonas Gahlert
 * \date    09.10.2026
 */

#pragma once

#include <concepts>
#include <ztl/ztl.hpp>
#include "transmission/decup_ein/base.hpp"

namespace transmission::decup_ein {

/**
 * DECUP_EIN base transmission
 *
 * \internal Overrides \ref Base::evaluate
 *
 */
struct Preamble : public Base {
  template<std::convertible_to<uint8_t>... Ts>
  Preamble(std::shared_ptr<internal::IConnection> conn, Ts&&... ts)
    : Base{conn, 0uz, std::forward<Ts>(ts)...} {}
  virtual ~Preamble() = default;

  virtual void execute() final;
};

} // namespace transmission::decup_ein
