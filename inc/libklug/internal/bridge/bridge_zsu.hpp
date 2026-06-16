/**
 * Internal ZSU Bridge
 *
 * \file    inc/libklug/internal/bridge/bridge_zsu.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <span>
#include <zsu/zsu.hpp>

namespace bridge {

struct ZSU {
  using AddressedBlock = std::pair<uint32_t, std::span<uint8_t const, 64uz>>;

  static zsu::File* read(std::filesystem::path path);
  static void release(zsu::File* file);

  static uint32_t blocks(zsu::Firmware const& firmware);

  static std::pair<uint32_t, std::span<uint8_t const, 64uz>>
  block(zsu::Firmware const& firmware, uint32_t index);

private:
  static unsigned int const _blockSize{64uz};
};

} // namespace bridge
