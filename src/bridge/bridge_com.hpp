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
 * Internal COM bridge
 *
 * \file    src/bridge/bridge_com.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <algorithm>
#include <expected>
#include "bridge_context.hpp"

namespace bridge {

/**
 * Bridge for the ULF_COM protocol
 *
 */
struct COM {
  COM(Context& ctx);

  std::string ping();
  bool reset();
  bool susiv2();
  bool mdu_ein();

private:
  Context& _ctx; ///< Bridge context
};

} // namespace bridge
