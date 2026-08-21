/**
 * Internal MDU_EIN bridge
 *
 * \file    inc/libklug/internal/bridge/bridge_mdu_ein.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <mdu/transfer_rate.hpp>
#include "bridge_context.hpp"
#include "bridge_worker.hpp"
#include "bridge_zpp.hpp"
#include "bridge_zsu.hpp"

namespace bridge {

/**
 * Bridge for the MDU_EIN protocol
 *
 */
struct MDU_EIN {
  MDU_EIN(Context& ctx, Worker& worker, ZPP& zpp, ZSU& zsu);

  // Entry
  std::expected<bool, err::Error> enterMDU();
  std::expected<bool, err::Error>
  enterDCCZSU(uint32_t id = 0uz, uint32_t sn = 0uz, bool done = true);
  std::expected<bool, err::Error> enterDCCZPP(uint32_t sn = 0uz,
                                              bool done = true);

  // General commands
  std::expected<bool, err::Error> ping(uint32_t sn = 0, uint32_t id = 0);
  std::expected<bool, err::Error>
  configTransferRate(mdu::TransferRate transfer_rate);
  std::expected<bool, err::Error> binaryTreeSearch();
  std::expected<uint8_t, err::Error> cvRead(uint16_t cv);
  std::expected<bool, err::Error> cvWrite(uint16_t cv, uint8_t value);
  std::expected<bool, err::Error> busy();

  // ZPP commands
  std::expected<bool, err::Error> zppValidQuery(std::string_view id,
                                                uint32_t size);
  std::expected<bool, err::Error> zppValidQuery(zpp::File* file);
  std::expected<bool, err::Error>
  zppLcDcQuery(std::span<uint8_t const, 4uz> dev_code);
  std::expected<bool, err::Error> zppLcDcQuery(zpp::File* file);
  std::expected<bool, err::Error> zppErase(uint32_t start_address,
                                           uint32_t end_address);
  std::expected<bool, err::Error> zppErase(zpp::File* file);
  std::expected<bool, err::Error>
  zppUpdate(uint32_t address, std::span<uint8_t const, 256uz> block);
  std::expected<bool, err::Error> zppUpdate(zpp::File* file, uint32_t index);
  std::expected<bool, err::Error> zppUpdateEnd(uint32_t start_address,
                                               uint32_t end_address);
  std::expected<bool, err::Error> zppUpdateEnd(zpp::File* file);
  std::expected<bool, err::Error> zppExitReset();

  // ZSU commands
  std::expected<bool, err::Error>
  zsuSalsa20IV(std::span<uint8_t const, 8uz> iv);
  std::expected<bool, err::Error> zsuSalsa20IV(zsu::Firmware const& firmware);
  std::expected<bool, err::Error> zsuErase(uint32_t start_address,
                                           uint32_t end_address);
  std::expected<bool, err::Error> zsuErase(zsu::Firmware const& firmware);
  std::expected<bool, err::Error>
  zsuUpdate(uint32_t address, std::span<uint8_t const, 64uz> block);
  std::expected<bool, err::Error> zsuUpdate(zsu::Firmware const& firmware,
                                            uint32_t index);
  std::expected<bool, err::Error>
  zsuCRC32Start(uint32_t start_address, uint32_t end_address, uint32_t crc);
  std::expected<bool, err::Error> zsuCRC32Start(zsu::Firmware const& firmware);
  std::expected<bool, err::Error> zsuCRC32Result();
  std::expected<bool, err::Error> zsuCRC32ResultExit();

private:
  Context& _ctx;   ///< Bridge context
  Worker& _worker; ///< Worker
  ZPP& _zpp;       ///< ZPP
  ZSU& _zsu;       ///< ZSU
};

} // namespace bridge
