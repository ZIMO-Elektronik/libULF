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
 * \file    src/bridge/bridge_mdu_ein.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <mdu/transfer_rate.hpp>
#include "bridge_context.hpp"
#include "bridge_zpp.hpp"
#include "bridge_zsu.hpp"

namespace bridge {

/**
 * Bridge for the MDU_EIN protocol
 *
 */
struct MDU_EIN {
  MDU_EIN(Context& ctx, ZPP& zpp, ZSU& zsu);

  // Entry
  bool enterMDU();
  bool enterDCCZSU(uint32_t id = 0uz, uint32_t sn = 0uz, bool done = true);
  bool enterDCCZPP(uint32_t sn = 0uz, bool done = true);

  // General commands
  bool ping(uint32_t sn = 0, uint32_t id = 0);
  bool configTransferRate(mdu::TransferRate transfer_rate);
  bool binaryTreeSearch();
  int cvRead(uint16_t cv);
  bool cvWrite(uint16_t cv, uint8_t value);
  bool busy();

  // ZPP commands
  bool zppValidQuery(std::string_view id, uint32_t size);
  bool zppValidQuery(zpp::File* file);
  bool zppLcDcQuery(std::span<uint8_t const, 4uz> dev_code);
  bool zppLcDcQuery(zpp::File* file);
  bool zppErase(uint32_t start_address, uint32_t end_address);
  bool zppErase(zpp::File* file);
  bool zppUpdate(uint32_t address, std::span<uint8_t const, 256uz> block);
  bool zppUpdate(zpp::File* file, uint32_t index);
  bool zppUpdateEnd(uint32_t start_address, uint32_t end_address);
  bool zppUpdateEnd(zpp::File* file);
  bool zppExitReset();

  // ZSU commands
  bool zsuSalsa20IV(std::span<uint8_t const, 8uz> iv);
  bool zsuSalsa20IV(zsu::Firmware const& firmware);
  bool zsuErase(uint32_t start_address, uint32_t end_address);
  bool zsuErase(zsu::Firmware const& firmware);
  bool zsuUpdate(uint32_t address, std::span<uint8_t const, 64uz> block);
  bool zsuUpdate(zsu::Firmware const& firmware, uint32_t index);
  bool
  zsuCRC32Start(uint32_t start_address, uint32_t end_address, uint32_t crc);
  bool zsuCRC32Start(zsu::Firmware const& firmware);
  bool zsuCRC32Result();
  bool zsuCRC32ResultExit();

private:
  Context& _ctx; ///< Bridge context
  ZPP& _zpp;     ///< ZPP
  ZSU& _zsu;     ///< ZSU
};

} // namespace bridge
