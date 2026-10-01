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
 * Internal ZSU bridge
 *
 * \file    src/bridge/bridge_zsu.cpp
 * \author  Jonas Gahlert
 * \date    12.05.2026
 */

#include "bridge_zsu.hpp"
#include <algorithm>
#include <cassert>

namespace bridge {

/**
 * Read ZSU File
 *
 * \param path  File path
 * \return zsu::File* Successful read
 * \return nullptr Error
 */
zsu::File* ZSU::read(std::filesystem::path path) {
  try {
    auto file{zsu::read(path)};

    // Pad to block size
    std::ranges::for_each(file.firmwares, [](zsu::Firmware& fw) {
      std::fill_n(std::back_inserter(fw.bin), fw.bin.size() % 64uz, 0u);
    });

    return new zsu::File(file);
  } catch (...) { return nullptr; }
}

/**
 * Free ZSU File
 *
 * \param file File
 */
void ZSU::release(zsu::File* file) { return delete file; }

/**
 * Get Firmware block count
 *
 * \param firmware  Firmware
 * \return uint32_t Block count
 */
uint32_t ZSU::blocks(zsu::Firmware const& firmware) {
  return firmware.bin.size() / _blockSize;
}

/**
 * Get block at index
 *
 * \param firmware  Firmware
 * \param index     Block index
 * \return std::pair<uint32_t, std::span<uint8_t const, 64uz>>
 */
ZSU::AddressedBlock ZSU::block(zsu::Firmware const& firmware, uint32_t index) {
  assert(index < blocks(firmware));
  return {index * _blockSize,
          std::span<uint8_t const, 64uz>{
            firmware.bin.data() + (index * _blockSize), _blockSize}};
}

} // namespace bridge
