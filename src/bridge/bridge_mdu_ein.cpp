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
 * Internal MDU_EIN bridge
 *
 * \file    src/bridge/bridge_mdu_ein.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge_mdu_ein.hpp"
#include <ulf/mdu_ein.hpp>
#include <utility>
#include "config.hpp"
#include "transmission/mdu_ein/base.hpp"
#include "transmission/mdu_ein/config_transfer_rate.hpp"
#include "transmission/mdu_ein/cv_read.hpp"
#include "transmission/mdu_ein/ping.hpp"
#include "ulf/cpp/ulf_error.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param ctx     Context
 */
MDU_EIN::MDU_EIN(Context& ctx, ZPP& zpp, ZSU& zsu)
  : _ctx{ctx}, _zpp{zpp}, _zsu{zsu} {}

/**
 * MDU entry
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::enterMDU() {
  std::array<uint8_t, 16u> payload{};
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(ulf::mdu_ein::Command::Entry, 0u, payload),
    internal::config::timeout::mdu_ein::enter_mdu};
  t.execute();
  return t.evaluateBool();
}

/**
 * DCC ZSU entry
 *
 * \param id    Decoder ID
 * \param sn    Decoder SN
 * \param done  true, if entry is done
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::enterDCCZSU(uint32_t id, uint32_t sn, bool done) {
  std::vector<uint8_t> payload{};
  auto it{std::back_inserter(payload)};
  ulf::mdu_ein::uint32_2data(id, it);
  ulf::mdu_ein::uint32_2data(sn, it);
  *it = done ? 0u : 1u;
  payload.resize(16);
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(
      ulf::mdu_ein::Command::Entry, 1u, std::span<uint8_t, 16u>{payload}),
    internal::config::timeout::mdu_ein::enter_dcc_zsu};
  t.execute();
  return t.evaluateBool();
}

/**
 * DCC ZPP entry
 *
 * \param sn    Decoder SN
 * \param done  True, if entry is done
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::enterDCCZPP(uint32_t sn, bool done) {
  std::vector<uint8_t> payload{};
  auto it{std::back_inserter(payload)};
  ulf::mdu_ein::uint32_2data(sn, it);
  *it = done ? 0u : 1u;
  payload.resize(16);
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(
      ulf::mdu_ein::Command::Entry, 2u, std::span<uint8_t, 16u>{payload}),
    internal::config::timeout::mdu_ein::enter_dcc_zpp};
  t.execute();
  return t.evaluateBool();
}

/**
 * Ping
 *
 * \param sn  Decoder SN
 * \param id  Decoder ID
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::ping(uint32_t sn, uint32_t id) {
  transmission::mdu_ein::Ping t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_ping_packet(sn, id)),
    internal::config::timeout::mdu_ein::ping};
  t.execute();
  return t.evaluateBool();
}

/**
 * Config Transfer Rate
 *
 * \param transfer_rate Transfer Rate
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::configTransferRate(mdu::TransferRate transfer_rate) {
  transmission::mdu_ein::ConfigTransferRate t{_ctx.connection, transfer_rate};
  t.execute();
  return t.evaluateBool();
}

/**
 * Binary Tree Search
 *
 * \todo Implement
 */
bool MDU_EIN::binaryTreeSearch() {
  using std::operator""sv;
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/**
 * Cv Read
 *
 * \param cv  Cv address
 *
 * \return int  A value >= 0 is the read value, < 0 is an invalid value
 *
 * \throws ulf_error   First error occurred
 */
int MDU_EIN::cvRead(uint16_t cv) {
  transmission::mdu_ein::CvRead t{_ctx.connection, cv};
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
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::cvWrite(uint16_t cv, uint8_t value) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_cv_write_packet(cv, value)),
    internal::config::timeout::mdu_ein::cv_write};
  t.execute();
  return t.evaluateBool();
}

/**
 * Busy
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::busy() {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_busy_packet()),
    internal::config::timeout::mdu_ein::busy};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Valid Query
 *
 * \param id    ZPP Id
 * \param size  ZPP size
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppValidQuery(std::string_view id, uint32_t size) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_valid_query_packet(id, size)),
    internal::config::timeout::mdu_ein::zpp_valid_query};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Valid Query
 *
 * \param file  ZPP File
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppValidQuery(zpp::File* file) {
  return zppValidQuery(file->id, file->flash.size());
}

/**
 * ZPP LC DC Query
 *
 * \param dev_code Developer code
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppLcDcQuery(std::span<uint8_t const, 4uz> dev_code) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_lc_dc_query_packet(dev_code)),
    internal::config::timeout::mdu_ein::zpp_lc_dc_query};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP LC DC Query
 *
 * \param file ZPP File
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppLcDcQuery(zpp::File* file) {
  return zppLcDcQuery(file->developer_code);
}

/**
 *  ZPP Erase
 *
 * \param start_address Start Address
 * \param end_address   End Address
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppErase(uint32_t start_address, uint32_t end_address) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zpp_erase_packet(start_address, end_address)),
    internal::config::timeout::mdu_ein::zpp_erase};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Erase
 *
 * \param file ZPP File
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppErase(zpp::File* file) {
  return zppErase(0uz, file->flash.size() - 1u);
}

/**
 * ZPP Update
 *
 * \param address Block Address
 * \param block   Block
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppUpdate(uint32_t address,
                        std::span<uint8_t const, 256uz> block) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_update_packet(address, block)),
    internal::config::timeout::mdu_ein::zpp_update};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Update
 *
 * \param file  ZPP File
 * \param index Block index
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppUpdate(zpp::File* file, uint32_t index) {
  auto const addressed_block{_zpp.block(file, index)};
  return zppUpdate(addressed_block.first, addressed_block.second);
}

/**
 * ZPP Update End
 *
 * \param start_address Start Address
 * \param end_address   End Address
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppUpdateEnd(uint32_t start_address, uint32_t end_address) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zpp_update_end_packet(start_address, end_address)),
    internal::config::timeout::mdu_ein::zpp_update_end};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Update End
 *
 * \param file  ZPP File
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppUpdateEnd(zpp::File* file) {
  return zppUpdateEnd(0uz, file->flash.size());
}

/**
 * ZPP Exit and Reset
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zppExitReset() {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_exit_reset_packet()),
    internal::config::timeout::mdu_ein::zpp_exit_reset};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU Init Salsa20
 *
 * \param iv  IV
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuSalsa20IV(std::span<uint8_t const, 8uz> iv) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_salsa20_iv_packet(iv)),
    internal::config::timeout::mdu_ein::zsu_salsa_20_iv};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU Init Salsa20
 *
 * \note If the Firmware does not contain an IV, the op will return an error
 *
 * \param firmware  Firmware
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuSalsa20IV(zsu::Firmware const& firmware) {
  if (!firmware.iv) return false;
  return zsuSalsa20IV(*firmware.iv);
}

/**
 * ZSU Erase
 *
 * \param start_address Start Address
 * \param end_address   End Address
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuErase(uint32_t start_address, uint32_t end_address) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zsu_erase_packet(start_address, end_address)),
    internal::config::timeout::mdu_ein::zsu_erase};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU Erase
 *
 * \param firmware  Firmware
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuErase(zsu::Firmware const& firmware) {
  return zsuErase(0uz, firmware.bin.size() - 1u);
}

/**
 * ZSU Update
 *
 * \param address Address
 * \param block   Block
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuUpdate(uint32_t address,
                        std::span<uint8_t const, 64uz> block) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_update_packet(address, block)),
    internal::config::timeout::mdu_ein::zsu_update};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU Update
 *
 * \param firmware Address
 * \param index    Block Index
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuUpdate(zsu::Firmware const& firmware, uint32_t index) {
  auto const addressed_block{_zsu.block(firmware, index)};
  return zsuUpdate(addressed_block.first, addressed_block.second);
}

/**
 * ZSU CRC32 start
 *
 * \param start_address Start address
 * \param end_address   End address
 * \param crc           CRC32
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuCRC32Start(uint32_t start_address,
                            uint32_t end_address,
                            uint32_t crc) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zsu_crc32_start_packet(start_address, end_address, crc)),
    internal::config::timeout::mdu_ein::zsu_crc32_start};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU CRC32 start
 *
 * \param firmware Firmware
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuCRC32Start(zsu::Firmware const& firmware) {
  return zsuCRC32Start(
    0uz, firmware.bin.size() - 1uz, mdu::crc32({firmware.bin}));
}

/**
 * ZSU CRC32 result
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuCRC32Result() {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_crc32_result_packet()),
    internal::config::timeout::mdu_ein::zsu_crc32_result};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU CRC32 result and exit
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool MDU_EIN::zsuCRC32ResultExit() {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_crc32_result_exit_packet()),
    internal::config::timeout::mdu_ein::zsu_crc32_result_exit};
  t.execute();
  return t.evaluateBool();
}

} // namespace bridge
