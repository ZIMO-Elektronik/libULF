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
 * \file    src/bridge/bridge_susiv2.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge_susiv2.hpp"
#include <ulf/susiv2.hpp>
#include <utility>
#include <zusi/zusi.hpp>
#include "config.hpp"
#include "klug/cpp/klug_error.hpp"
#include "transmission/susiv2/base.hpp"
#include "transmission/susiv2/cv_read.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param ctx     Context
 */
SUSIV2::SUSIV2(Context& ctx, ZPP& zpp) : _ctx{ctx}, _zpp{zpp} {}

/**
 * Cv Read
 *
 * \param cv  Cv address
 *
 * \return int  A value >= 0 is the read value, < 0 is an invalid value
 *
 * \throws klug_error   First error occurred
 */
int SUSIV2::cvRead(uint16_t cv) {
  transmission::susiv2::CvRead t{_ctx.connection, 2000uz, cv};
  t.execute();
  return t.evaluateValue();
}

/**
 * Cv Write
 *
 * \param cv    Cv address
 * \param value Cv value
 *
 * \return bool true, if successful, false else
 *
 * \throws klug_error   First error occurred
 */
bool SUSIV2::cvWrite(uint16_t cv, uint8_t value) {
  using std::operator""sv;
  throw libklug::klug_error{libklug::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/**
 * ZPP erase
 *
 * \return bool true, if successful, false else
 *
 * \throws klug_error   First error occurred
 */
bool SUSIV2::zppErase() {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_zpp_erase_packet()),
    internal::config::timeout::susiv2::zpp_erase};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP write
 *
 * \param address Block address
 * \param block   Block
 *
 * \return bool true, if successful, false else
 *
 * \throws klug_error   First error occurred
 */
bool SUSIV2::zppWrite(uint32_t address, std::span<uint8_t const> block) {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_zpp_write_packet(block.size() - 1u, address, block)),
    internal::config::timeout::susiv2::zpp_erase};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP write from file
 *
 * \param file  ZPP file
 * \param index Block index
 *
 * \return bool true, if successful, false else
 *
 * \throws klug_error   First error occurred
 */
bool SUSIV2::zppWrite(zpp::File* file, uint32_t index) {
  auto const block{_zpp.block(file, index)};
  return zppWrite(block.first, block.second);
}

/**
 * Feature request
 *
 * \return bool true, if successful, false else
 *
 * \throws klug_error   First error occurred
 */
bool SUSIV2::features() {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_features_packet()),
    internal::config::timeout::susiv2::features};
  t.execute();
  return t.evaluateBool();
}

/**
 * Exit
 *
 * \param reboot    Decoder reboot
 * \param cv8_reset Decoder Cv8 reset
 *
 * \return bool true, if successful, false else
 *
 * \throws klug_error   First error occurred
 */
bool SUSIV2::exit(bool reboot, bool cv8_reset) {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_exit_packet(0xFC | (reboot << 0u) | (cv8_reset << 1u))),
    internal::config::timeout::susiv2::exit};
  t.execute();
  return t.evaluateBool();
}

/**
 * LC DC query
 *
 * \param dev_code Developer code
 *
 * \return bool true, if successful, false else
 *
 * \throws klug_error   First error occurred
 */
bool SUSIV2::zppLcDcQuery(uint32_t dev_code) {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_zpp_lc_dc_query_packet(dev_code)),
    internal::config::timeout::susiv2::zpp_lc_dc_query};
  t.execute();
  return t.evaluateBool();
}

/**
 * LC DC query
 *
 * \param file  ZPP file
 *
 * \return bool true, if successful, false else
 *
 * \throws klug_error   First error occurred
 */
bool SUSIV2::zppLcDcQuery(zpp::File* file) {
  return zppLcDcQuery(zusi::data2uint32(file->developer_code.data()));
}

} // namespace bridge
