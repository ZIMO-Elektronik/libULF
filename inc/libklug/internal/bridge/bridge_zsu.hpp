/**
 * Internal ZSU Bridge
 *
 * \file    inc/libklug/internal/bridge/bridge_zsu.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <zsu/zsu.hpp>

namespace bridge {

struct ZSU {
  using AddressedBlock = std::pair<uint32_t, std::span<uint8_t const, 64uz>>;

  zsu::File* read(std::filesystem::path path);
  void release(zsu::File* file);

  uint32_t blocks(zsu::Firmware const& firmware);

  AddressedBlock block(zsu::Firmware const& firmware, uint32_t index);

private:
  size_t const _blockSize{64uz}; ///< Block size
};

} // namespace bridge
