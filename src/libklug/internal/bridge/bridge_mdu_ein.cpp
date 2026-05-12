/**
 * Internal MDU_EIN bridge
 *
 * \file    src/libklug/internal/bridge/bridge_mdu_ein.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_mdu_ein.hpp"
#include <ulf/mdu_ein.hpp>
#include "libklug/internal/transmission/mdu_ein/base.hpp"
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
bool MDU_EIN::enterMDU() {
  std::array<uint8_t, 16u> payload{};
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(ulf::mdu_ein::Command::Entry, 0u, payload),
    2000u);
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
bool MDU_EIN::enterDCCZSU(uint32_t id, uint32_t sn, bool done) {
  std::vector<uint8_t> payload{};
  auto it{std::back_inserter(payload)};
  ulf::mdu_ein::uint32_2data(id, it);
  ulf::mdu_ein::uint32_2data(sn, it);
  *it = done ? 0u : 1u;
  payload.resize(16);
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(
      ulf::mdu_ein::Command::Entry, 1u, std::span<uint8_t, 16u>{payload}),
    2000u);
}

/**
 * DCC ZPP entry (async)
 *
 * \param sn    Decoder SN
 * \param done  True, if entry is done
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::enterDCCZPP(uint32_t sn, bool done) {
  std::vector<uint8_t> payload{};
  auto it{std::back_inserter(payload)};
  ulf::mdu_ein::uint32_2data(sn, it);
  *it = done ? 0u : 1u;
  payload.resize(16);
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(
      ulf::mdu_ein::Command::Entry, 2u, std::span<uint8_t, 16u>{payload}),
    2000u);
}

/**
 * Ping (async)
 *
 * \param sn  Decoder SN
 * \param id  Decoder ID
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::ping(uint32_t sn, uint32_t id) {
  return _worker.emplace<transmission::mdu_ein::Ping>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_ping_packet(sn, id)),
    2000u);
}

/**
 * Config Transfer Rate (async)
 *
 * \param transfer_rate Transfer Rate
 * \return true
 * \return false
 * \todo  Impement, maybe also perform special command?
 */
bool MDU_EIN::configTransferRate(mdu::TransferRate transfer_rate) {
  assert(false);
  return false;
}

/**
 * Binary Tree Search (async)
 *
 * \return true
 * \return false
 * \todo Implement
 */
bool MDU_EIN::binaryTreeSearch() {
  assert(false);
  return false;
}

/**
 * Cv Read (async)
 *
 * \param cv  Cv address
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::cvRead(uint16_t cv) {
  return _worker.emplace<transmission::mdu_ein::CvRead>(_ctx.connection, cv);
}

/**
 * Cv Write (async)
 *
 * \param cv    Cv address
 * \param value Cv value
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::cvWrite(uint16_t cv, uint8_t value) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_cv_write_packet(cv, value)),
    2000u);
}

/**
 * Busy (async)
 *
 * \return true  Success
 * \return false Busy
 */
bool MDU_EIN::busy() {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_busy_packet()),
    2000u);
}

/**
 * ZPP Valid Query (async)
 *
 * \param id    ZPP Id
 * \param size  ZPP size
 * \todo Implement, Library implementation of packet factory also missing
 */
bool MDU_EIN::zppValidQuery(std::string_view id, uint32_t size) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_valid_query_packet(id, size)),
    2000uz);
}

/**
 * ZPP Valid Query (async)
 *
 * \param file  ZPP File
 * \return Forwarded
 */
bool MDU_EIN::zppValidQuery(zpp::File* file) {
  return zppValidQuery(file->id, file->flash.size());
}

/**
 * ZPP LC DC Query (async)
 *
 * \param dev_code Developer code
 * \return false
 * \todo Implement, Library implementation of packet factory also missing
 */
bool MDU_EIN::zppLcDcQuery(std::span<uint8_t const, 4uz> dev_code) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_lc_dc_query_packet(dev_code)),
    2000uz);
}

/**
 * ZPP LC DC Query (async)
 *
 * \param file ZPP File
 * \return Forwarded
 */
bool MDU_EIN::zppLcDcQuery(zpp::File* file) {
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
bool MDU_EIN::zppErase(uint32_t start_address, uint32_t end_address) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zpp_erase_packet(start_address, end_address)),
    2000uz);
}

/**
 * ZPP Erase (async)
 *
 * \param file ZPP File
 * \return Forwarded
 */
bool MDU_EIN::zppErase(zpp::File* file) {
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
bool MDU_EIN::zppUpdate(uint32_t address,
                        std::span<uint8_t const, 256uz> block) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_update_packet(address, block)),
    2000uz);
}

/**
 * ZPP Update (async)
 *
 * \param file  ZPP File
 * \param index Block index
 * \return Forwarded
 */
bool MDU_EIN::zppUpdate(zpp::File* file, uint32_t index) {
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
bool MDU_EIN::zppUpdateEnd(uint32_t start_address, uint32_t end_address) {
  assert(false);
  return false;
}

/**
 * ZPP Update End (async)
 *
 * \param file  ZPP File
 * \return Forwarded
 */
bool MDU_EIN::zppUpdateEnd(zpp::File* file) {
  return zppUpdate(0uz, file->flash.size() - 1uz);
}

/**
 * ZPP Exit and Reset (async)
 *
 * \return false
 * \todo Implement, Library implementation of packet factory also missing
 */
bool MDU_EIN::zppExitReset() {
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
bool MDU_EIN::zsuSalsa20IV(std::span<uint8_t const, 8uz> iv) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_salsa20_iv_packet(iv)),
    2000u);
}

/**
 * ZSU Init Salsa20 (asnyc)
 *
 * \note If the Firmware does not contain an IV, the op will return an error
 *
 * \param firmware  Firmware
 * \return Forwarded
 */
bool MDU_EIN::zsuSalsa20IV(zsu::Firmware const& firmware) {
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
bool MDU_EIN::zsuErase(uint32_t start_address, uint32_t end_address) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zsu_erase_packet(start_address, end_address)),
    2000u);
}

/**
 * ZSU Erase (async)
 *
 * \param firmware  Firmware
 * \return true  Forwarded
 */
bool MDU_EIN::zsuErase(zsu::Firmware const& firmware) {
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
bool MDU_EIN::zsuUpdate(uint32_t address,
                        std::span<uint8_t const, 64uz> block) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_zsu_update_packet(address, block)),
    2000u);
}

/**
 * ZSU Update (async)
 *
 * \param firmware Address
 * \param index    Block Index
 * \return Forwarded
 */
bool MDU_EIN::zsuUpdate(zsu::Firmware const& firmware, uint32_t index) {
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
bool MDU_EIN::zsuCRC32Start(uint32_t start_address,
                            uint32_t end_address,
                            uint32_t crc) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(
      mdu::make_zsu_crc32_start_packet(start_address, end_address, crc)),
    2000uz);
}

/**
 * ZSU CRC32 start (async)
 *
 * \param firmware Firmware
 * \return Forwarded
 */
bool MDU_EIN::zsuCRC32Start(zsu::Firmware const& firmware) {
  return zsuCRC32Start(
    0uz, firmware.bin.size() - 1uz, mdu::crc32({firmware.bin}));
}

/**
 * ZSU CRC32 result (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::zsuCRC32Result() {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection, mdu::make_zsu_crc32_result_packet(), 2000uz);
}

/**
 * ZSU CRC32 result and exit (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::zsuCRC32ResultExit() {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection, mdu::make_zsu_crc32_result_exit_packet(), 2000uz);
}

} // namespace bridge
