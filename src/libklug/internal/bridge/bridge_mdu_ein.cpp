/**
 * Internal MDU_EIN bridge
 *
 * \file    src/libklug/internal/bridge/bridge_mdu_ein.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_mdu_ein.hpp"
#include <ulf/mdu_ein.hpp>
#include "config.hpp"
#include "libklug/internal/transmission/mdu_ein/base.hpp"
#include "libklug/internal/transmission/mdu_ein/config_transfer_rate.hpp"
#include "libklug/internal/transmission/mdu_ein/cv_read.hpp"
#include "libklug/internal/transmission/mdu_ein/ping.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param ctx     Context
 * \param worker  Worker
 */
MDU_EIN::MDU_EIN(Context& ctx, Worker& worker, ZPP& zpp, ZSU& zsu)
  : _ctx{ctx}, _worker{worker}, _zpp{zpp}, _zsu{zsu} {}

/**
 * MDU entry (async)
 *
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> MDU_EIN::enterMDU() {
  std::array<uint8_t, 16u> payload{};
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(ulf::mdu_ein::Command::Entry, 0u, payload),
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * DCC ZSU entry (async)
 *
 * \param id    Decoder ID
 * \param sn    Decoder SN
 * \param done  true, if entry is done
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error>
MDU_EIN::enterDCCZSU(uint32_t id, uint32_t sn, bool done) {
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
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * DCC ZPP entry (async)
 *
 * \param sn    Decoder SN
 * \param done  True, if entry is done
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> MDU_EIN::enterDCCZPP(uint32_t sn, bool done) {
  std::vector<uint8_t> payload{};
  auto it{std::back_inserter(payload)};
  ulf::mdu_ein::uint32_2data(sn, it);
  *it = done ? 0u : 1u;
  payload.resize(16);
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(
      ulf::mdu_ein::Command::Entry, 2u, std::span<uint8_t, 16u>{payload}),
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * Ping (async)
 *
 * \param sn  Decoder SN
 * \param id  Decoder ID
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> MDU_EIN::ping(uint32_t sn, uint32_t id) {
  transmission::mdu_ein::Ping t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_ping_packet(sn, id)),
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * Config Transfer Rate (async)
 *
 * \param transfer_rate Transfer Rate
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error>
MDU_EIN::configTransferRate(mdu::TransferRate transfer_rate) {
  transmission::mdu_ein::ConfigTransferRate t{_ctx.connection, transfer_rate};
  t.execute();
  return t.evaluateBool();
}

/**
 * Binary Tree Search (async)
 *
 * \return true
 * \return false
 * \todo Implement
 */
std::expected<bool, err::Error> MDU_EIN::binaryTreeSearch() {
  assert(false);
  return std::unexpected(err::Error::unknown);
}

/**
 * Cv Read (async)
 *
 * \param cv  Cv address
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> MDU_EIN::cvRead(uint16_t cv) {
  transmission::mdu_ein::CvRead t{_ctx.connection, cv};
  t.execute();
  return t.evaluateBool();
}

/**
 * Cv Write (async)
 *
 * \param cv    Cv address
 * \param value Cv value
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> MDU_EIN::cvWrite(uint16_t cv, uint8_t value) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_cv_write_packet(cv, value)),
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * Busy (async)
 *
 * \return true  Success
 * \return false Busy
 */
std::expected<bool, err::Error> MDU_EIN::busy() {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_busy_packet()),
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Valid Query (async)
 *
 * \param id    ZPP Id
 * \param size  ZPP size
 * \todo Implement, Library implementation of packet factory also missing
 */
std::expected<bool, err::Error> MDU_EIN::zppValidQuery(std::string_view id,
                                                       uint32_t size) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_valid_query_packet(id, size)),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Valid Query (async)
 *
 * \param file  ZPP File
 * \return Forwarded
 */
std::expected<bool, err::Error> MDU_EIN::zppValidQuery(zpp::File* file) {
  return zppValidQuery(file->id, file->flash.size());
}

/**
 * ZPP LC DC Query (async)
 *
 * \param dev_code Developer code
 * \return false
 * \todo Implement, Library implementation of packet factory also missing
 */
std::expected<bool, err::Error>
MDU_EIN::zppLcDcQuery(std::span<uint8_t const, 4uz> dev_code) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_lc_dc_query_packet(dev_code)),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP LC DC Query (async)
 *
 * \param file ZPP File
 * \return Forwarded
 */
std::expected<bool, err::Error> MDU_EIN::zppLcDcQuery(zpp::File* file) {
  return zppLcDcQuery(file->developer_code);
}

/**
 *  ZPP Erase (async)
 *
 * \param start_address Start Address
 * \param end_address   End Address
 * \return false
 * \todo Implement, Library implementation of packet factory also missing
 */
std::expected<bool, err::Error> MDU_EIN::zppErase(uint32_t start_address,
                                                  uint32_t end_address) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zpp_erase_packet(start_address, end_address)),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Erase (async)
 *
 * \param file ZPP File
 * \return Forwarded
 */
std::expected<bool, err::Error> MDU_EIN::zppErase(zpp::File* file) {
  return zppErase(0uz, file->flash.size() - 1u);
}

/**
 * ZPP Update (async)
 *
 * \param address Block Address
 * \param block   Block
 * \return false
 * \todo Implement, Library implementation of packet factory also missing
 */
std::expected<bool, err::Error>
MDU_EIN::zppUpdate(uint32_t address, std::span<uint8_t const, 256uz> block) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_update_packet(address, block)),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Update (async)
 *
 * \param file  ZPP File
 * \param index Block index
 * \return Forwarded
 */
std::expected<bool, err::Error> MDU_EIN::zppUpdate(zpp::File* file,
                                                   uint32_t index) {
  auto const addressed_block{_zpp.block(file, index)};
  return zppUpdate(addressed_block.first, addressed_block.second);
}

/**
 * ZPP Update End (async)
 *
 * \param start_address Start Address
 * \param end_address   End Address
 * \return false
 * \todo Implement, Library implementation of packet factory also missing
 */
std::expected<bool, err::Error> MDU_EIN::zppUpdateEnd(uint32_t start_address,
                                                      uint32_t end_address) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_exit_reset_packet()),
    internal::config::timeout::mdu_ein::zpp_exit_reset};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP Update End (async)
 *
 * \param file  ZPP File
 * \return Forwarded
 */
std::expected<bool, err::Error> MDU_EIN::zppUpdateEnd(zpp::File* file) {
  return zppUpdateEnd(0uz, file->flash.size() - 1uz);
}

/**
 * ZPP Exit and Reset (async)
 *
 * \return false
 * \todo Implement, Library implementation of packet factory also missing
 */
std::expected<bool, err::Error> MDU_EIN::zppExitReset() {
  assert(false);
  return false;
}

/**
 * ZSU Init Salsa20 (async)
 *
 * \param iv  IV
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error>
MDU_EIN::zsuSalsa20IV(std::span<uint8_t const, 8uz> iv) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_salsa20_iv_packet(iv)),
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU Init Salsa20 (asnyc)
 *
 * \note If the Firmware does not contain an IV, the op will return an error
 *
 * \param firmware  Firmware
 * \return Forwarded
 */
std::expected<bool, err::Error>
MDU_EIN::zsuSalsa20IV(zsu::Firmware const& firmware) {
  if (!firmware.iv) return false;
  return zsuSalsa20IV(*firmware.iv);
}

/**
 * ZSU Erase (async)
 *
 * \param start_address Start Address
 * \param end_address   End Address
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> MDU_EIN::zsuErase(uint32_t start_address,
                                                  uint32_t end_address) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zsu_erase_packet(start_address, end_address)),
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU Erase (async)
 *
 * \param firmware  Firmware
 * \return true  Forwarded
 */
std::expected<bool, err::Error>
MDU_EIN::zsuErase(zsu::Firmware const& firmware) {
  return zsuErase(0uz, firmware.bin.size() - 1u);
}

/**
 * ZSU Update (async)
 *
 * \param address Address
 * \param block   Block
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error>
MDU_EIN::zsuUpdate(uint32_t address, std::span<uint8_t const, 64uz> block) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_update_packet(address, block)),
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU Update (async)
 *
 * \param firmware Address
 * \param index    Block Index
 * \return Forwarded
 */
std::expected<bool, err::Error>
MDU_EIN::zsuUpdate(zsu::Firmware const& firmware, uint32_t index) {
  auto const addressed_block{_zsu.block(firmware, index)};
  return zsuUpdate(addressed_block.first, addressed_block.second);
}

/**
 * ZSU CRC32 start (async)
 *
 * \param start_address Start address
 * \param end_address   End address
 * \param crc           CRC32
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> MDU_EIN::zsuCRC32Start(uint32_t start_address,
                                                       uint32_t end_address,
                                                       uint32_t crc) {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zsu_crc32_start_packet(start_address, end_address, crc)),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU CRC32 start (async)
 *
 * \param firmware Firmware
 * \return Forwarded
 */
std::expected<bool, err::Error>
MDU_EIN::zsuCRC32Start(zsu::Firmware const& firmware) {
  return zsuCRC32Start(
    0uz, firmware.bin.size() - 1uz, mdu::crc32({firmware.bin}));
}

/**
 * ZSU CRC32 result (async)
 *
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> MDU_EIN::zsuCRC32Result() {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_crc32_result_packet()),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZSU CRC32 result and exit (async)
 *
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> MDU_EIN::zsuCRC32ResultExit() {
  transmission::mdu_ein::Base t{
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_crc32_result_exit_packet()),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

} // namespace bridge
