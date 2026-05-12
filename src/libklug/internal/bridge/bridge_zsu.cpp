/**
 * Internal ZSU bridge
 *
 * \file    src/libklug/internal/bridge/bridge_zsu.cpp
 * \author  Jonas Gahlert
 * \date    12.05.2026
 */

#include "libklug/internal/bridge/bridge_zsu.hpp"
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
    return new zsu::File(zsu::read(path));
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
  return (firmware.bin.size() + _blockSize - 1uz) / _blockSize;
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
