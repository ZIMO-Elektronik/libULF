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
 * Internal DECUP_EIN bridge
 *
 * \file    src/bridge/bridge_decup_ein.hpp
 * \author  Jonas Gahlert
 * \date    09.10.2026
 */

#pragma once

#include "bridge_context.hpp"
#include "bridge_zpp.hpp"
#include "bridge_zsu.hpp"

namespace bridge {

/**
 * Bridge for the DECUP_EIN protocol
 *
 */
struct DECUP_EIN {
  DECUP_EIN(Context& ctx, ZPP& zpp, ZSU& zsu);

  // Entry
  bool enterZSU();
  bool enterZPP();

  // ZSU
  bool zsuDecoderId(uint8_t id);
  bool zsuPageCount(zsu::Firmware const& firmware);
  bool zsuSecurityBytes();
  bool zsuBlocks(zsu::Firmware const& firmware, uint32_t index);

  // ZPP
  uint8_t cvRead(uint16_t cv);
  bool cvWrite(uint16_t cv, uint8_t value);
  uint8_t decoderId();
  bool zppFlashErase();
  bool zppWrite(zpp::File* file, uint32_t index);

private:
  Context& _ctx; ///< Bridge context
  ZPP& _zpp;     ///< ZPP
  ZSU& _zsu;     ///< ZSU
};

} // namespace bridge
