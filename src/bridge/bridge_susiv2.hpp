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
 * Internal SUSIV2 bridge
 *
 * \file    inc/libklug/internal/bridge/bridge_susiv2.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <expected>
#include <zpp/zpp.hpp>
#include "bridge_context.hpp"
#include "bridge_zpp.hpp"

namespace bridge {

/**
 * Bridge for the SUSIV2 protocol
 *
 */
struct SUSIV2 {
  SUSIV2(Context& ctx, ZPP& zpp);

  int cvRead(uint16_t cv);
  bool cvWrite(uint16_t cv, uint8_t value);
  bool zppErase();
  bool zppWrite(uint32_t address, std::span<uint8_t const> block);
  bool zppWrite(zpp::File* file, uint32_t index);
  bool features();
  bool exit(bool reboot, bool cv8_reset);
  bool zppLcDcQuery(uint32_t dev_code);
  bool zppLcDcQuery(zpp::File* file);

private:
  Context& _ctx; ///< Bridge context
  ZPP& _zpp;     ///< ZPP bridge
};

} // namespace bridge
