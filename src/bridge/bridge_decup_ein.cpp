/**
 * Copyright (C) 2026 ZIMO Elektronik
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
 * Internal DECUP bridge
 *
 * \file    src/bridge/bridge_decup_ein.cpp
 * \author  Jonas Gahlert
 * \date    09.11.2026
 */

#include "bridge_decup_ein.hpp"
#include <decup/decup.hpp>
#include "transmission/decup_ein/base.hpp"
#include "transmission/decup_ein/cv_read.hpp"
#include "transmission/decup_ein/preamble.hpp"
#include "transmission/decup_ein/security_bytes.hpp"
#include "ulf/cpp/ulf_error.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param [in] ctx Context
 * \param [in] zpp ZPP reference
 * \param [in] zsu ZSU reference
 */
DECUP_EIN::DECUP_EIN(Context& ctx, ZPP& zpp, ZSU& zsu)
  : _ctx{ctx}, _zpp{zpp}, _zsu{zsu} {}

/**
 * Enters decoder(-s) into update mode
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::enterZSU() {
  transmission::decup_ein::Preamble{_ctx.connection, 0xBFu, 0xEFu}.execute();
  return true;
}

/**
 * Enters decoder(-s) into sound load mode
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::enterZPP() {
  transmission::decup_ein::Preamble{_ctx.connection, 0xBFu, 0xEFu, 0xEFu}
    .execute();
  return true;
}

/**
 * Transmits decoder ID (must be done for each available id seperately)
 *
 * \param id  Decoder ID
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::zsuDecoderId(uint8_t id) {
  transmission::decup_ein::Base t{_ctx.connection, 100u, id};
  try {
    t.execute();
    return !t.evaluateBool();
  } catch (libulf::ulf_error const& e) { return false; }
}

/**
 * Transmits the firmware page count
 *
 * \param firmware Firmware
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::zsuPageCount(zsu::Firmware const& firmware) {
  auto const page_count{
    (firmware.bin.size() +
     decup::decoder_id2bootloader_size(static_cast<uint8_t>(firmware.id))) /
      256u -
    1u};
  transmission::decup_ein::Base t{
    _ctx.connection, 100u, static_cast<uint8_t>(page_count)};
  t.execute();
  return t.evaluateBool();
}

/**
 * Transmits the security bytes
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::zsuSecurityBytes() {
  transmission::decup_ein::SecurityBytes t{_ctx.connection, 100u};
  t.execute();
  return t.evaluateBool();
}

/**
 * Transmits a firmware block
 *
 * \param firmware  Firmware
 * \param index     Block index
 *
 * \todo
 * Maybe some parts of packet assembly could be outsourced to library...
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::zsuBlocks(zsu::Firmware const& firmware, uint32_t index) {
  uint32_t const block_size{decup::decoder_id2block_size(firmware.id)};

  std::vector<uint8_t> m{static_cast<uint8_t>(index)};
  m.reserve(70uz);
  std::ranges::copy(std::span<uint8_t const>{firmware.bin}.subspan(
                      index * block_size, block_size),
                    std::back_inserter(m));
  uint8_t crc{0};
  std::ranges::for_each(m, [&crc](uint8_t const b) { crc ^= b; });
  m.push_back(crc);

  transmission::decup_ein::Base t{_ctx.connection, m, 100u};
  t.execute();
  return t.evaluateBool();
}

/**
 * Reads a cv
 *
 * \param cv  CV address
 *
 * \return value
 *
 * \throws ulf_error   First error occurred
 */
uint8_t DECUP_EIN::cvRead(uint16_t cv) {
  transmission::decup_ein::CvRead t{_ctx.connection, cv};
  t.execute();
  return t.evaluateValue();
}

/**
 * Writes a cv
 *
 * \param cv    CV address
 * \param value CV value
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::cvWrite(uint16_t cv, uint8_t value) {
  bool returnValue;
  return returnValue;
}

/**
 * Reads the decoder ID
 *
 * \return ID
 *
 * \throws ulf_error   First error occurred
 */
uint8_t DECUP_EIN::decoderId() {
  uint8_t returnValue;
  return returnValue;
}

/**
 * Erases sound flash
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::zppFlashErase() {
  bool returnValue;
  return returnValue;
}

/**
 * Writes a sound flash block
 *
 * \param file  ZPP file
 * \param index Block index
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::zppWrite(zpp::File* file, uint32_t index) {
  bool returnValue;
  return returnValue;
}

} // namespace bridge
