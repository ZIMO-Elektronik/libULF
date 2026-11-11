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
#include "config.hpp"
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
  transmission::decup_ein::Base t{
    _ctx.connection, internal::config::timeout::decup_ein::zsu_decoder_id, id};
  try {
    t.execute();
    return t.evaluateBool();
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
  return !t.evaluateBool();
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
  return !t.evaluateBool();
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
  std::vector<uint8_t> m{static_cast<uint8_t>(index)};
  m.reserve(1u                         // Index
            + ZSU::blockSize(firmware) // Block
            + 1u                       // XOR
  );
  std::ranges::copy(ZSU::block(firmware, index), std::back_inserter(m));
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
  decup::Packet v{
    static_cast<uint8_t>(decup::Command::CvWrite), // Command
    0xAAu,                                         // Security byte
    static_cast<uint8_t>(cv >> 8u),                //
    static_cast<uint8_t>(cv >> 0u),                // Cv Address
  };

  v.push_back(decup::crc8(v)); // Checksum
  v.push_back(value);          // Value

  transmission::decup_ein::Base t{
    _ctx.connection, std::span<uint8_t const>{v.data(), v.size()}, 100u};
  t.execute();
  return !t.evaluateBool();
}

/**
 * Reads the decoder ID
 *
 * \return ID
 *
 * \throws ulf_error   First error occurred
 */
uint8_t DECUP_EIN::decoderId() {
  using std::operator""sv;
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/**
 * Erases sound flash
 *
 * \return bool true, if successful, false else
 *
 * \throws ulf_error   First error occurred
 */
bool DECUP_EIN::zppFlashErase() {
  transmission::decup_ein::Base t{
    _ctx.connection,
    internal::config::timeout::decup_ein::zpp_flash_erase,
    static_cast<uint8_t>(decup::Command::DeleteFlash),
    0x55u,
    0xFFu,
    0xFFu,
  };
  t.execute();
  return !t.evaluateBool();
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
  decup::Packet p{
    0x05u,
    0x55u,
    static_cast<uint8_t>(index >> 8u),
    static_cast<uint8_t>(index >> 0u),
  };
  std::ranges::copy(ZPP::block(file, index), std::back_inserter(p));

  bool returnValue;
  return returnValue;
}

} // namespace bridge
