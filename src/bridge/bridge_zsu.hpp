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
 * Internal ZSU Bridge
 *
 * \file    src/bridge/bridge_zsu.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <span>
#include <zsu/zsu.hpp>

namespace bridge {

struct ZSU {
  using Block = std::span<uint8_t const>;
  using AddressedBlock = std::pair<uint32_t, Block>;

  static zsu::File* read(std::filesystem::path path);
  static void release(zsu::File* file);

  static uint32_t blockCount(zsu::Firmware const& firmware);
  static uint32_t blockSize(zsu::Firmware const& firmware);

  static Block block(zsu::Firmware const& firmware, uint32_t index);
  static AddressedBlock addressedBlock(zsu::Firmware const& firmware,
                                       uint32_t index);
};

} // namespace bridge
