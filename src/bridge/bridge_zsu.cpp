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
uint32_t ZSU::blockCount(zsu::Firmware const& firmware) {
  return firmware.bin.size() / blockSize(firmware);
}

/**
 * Get Firmware block size
 *
 * \param firmware  Firmware
 * \return uint32_t Block size
 */
uint32_t ZSU::blockSize(zsu::Firmware const& firmware) {
  return firmware.type == '1'   ? 32uz  // PIC16
         : firmware.type == '2' ? 64uz  // PIC18
                                : 64uz; // STM32
}

/**
 * Get a firmware block
 *
 * \param firmware  Firmware
 * \param index     Block index
 *
 * \return Block
 */
ZSU::Block ZSU::block(zsu::Firmware const& firmware, uint32_t index) {
  assert(index < blockCount(firmware));
  auto const block_size{blockSize(firmware)};
  return {firmware.bin.data() + (index * block_size), block_size};
}

/**
 * Get addressed block at index
 *
 * \param firmware  Firmware
 * \param index     Block index
 *
 * \return Addressed block
 */
ZSU::AddressedBlock ZSU::addressedBlock(zsu::Firmware const& firmware,
                                        uint32_t index) {
  return {index * blockSize(firmware), block(firmware, index)};
}

} // namespace bridge
