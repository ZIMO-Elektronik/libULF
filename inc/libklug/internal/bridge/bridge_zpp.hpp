/**
 * Internal ZPP bridge
 *
 * \file    inc/libklug/internal/bridge/bridge_zpp.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <span>
#include <zpp/zpp.hpp>

namespace bridge {

struct ZPP {
  using AddressedBlock = std::pair<uint32_t, std::span<uint8_t const, 256uz>>;

  zpp::File* read(std::filesystem::path path);
  void release(zpp::File* file);

  unsigned int blocks(zpp::File* file);
  unsigned int cvs(zpp::File* file);

  AddressedBlock block(zpp::File* file, unsigned int block);

  std::string_view author(zpp::File* file);
  std::string_view email(zpp::File* file);

private:
  unsigned long const _blockSize{256uz};
};

} // namespace bridge
