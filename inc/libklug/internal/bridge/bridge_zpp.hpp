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

  static zpp::File* read(std::filesystem::path path);
  static void release(zpp::File* file);

  static unsigned int blocks(zpp::File* file);
  static unsigned int cvs(zpp::File* file);

  static AddressedBlock block(zpp::File* file, unsigned int block);

  static std::string_view author(zpp::File* file);
  static std::string_view email(zpp::File* file);

private:
  static unsigned long const _blockSize{256uz};
};

} // namespace bridge
