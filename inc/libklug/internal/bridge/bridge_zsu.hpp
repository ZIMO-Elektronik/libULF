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
  zsu::File* read(std::filesystem::path path);
  void release(zsu::File* file);

  unsigned int blocks(zsu::File* file);

  std::span<uint8_t> block(zsu::File* file, unsigned int block);
};

} // namespace bridge
