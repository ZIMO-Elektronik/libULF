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
 * Libulf C++ wrapper
 *
 * This is a simple wrapper header, that wraps the LibULF C API in flat C++
 * classes with. The hierarchy remains the same.
 *
 * Pretty much every class here can either be constructed with their own default
 * argument, or move constructed. Anything else may (or will) result in severe
 * state inconsistencies, crashes, memory leaks and or world destruction. Things
 * that happen when you do things which are not recommended.
 *
 * And yes, moving an object and then using the moved object WILL crash the APP.
 *
 * \file    include/ulf/cpp/libulf.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cassert>
#include <exception>
#include <expected>
#include <filesystem>
#include <functional>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include "error.hpp"
#include "error2string.hpp"
#include "ulf/c/libulf.h"
#include "ulf_error.hpp"

namespace {

/**
 * Execute helper
 *
 * \note
 * Wraps the C API calls into an explicit return value with an exception on
 * error.
 *
 * \tparam T  Return type
 * \tparam F  Function type
 * \tparam Ts Parameter type list
 * \param f   Function (from C API)
 * \param ts  Parameter pack
 *
 * \return API result
 */
template<typename T, typename F, typename... Ts>
constexpr T execute(F f, Ts&&... ts) {
  T r{};
  if (auto const e{std::invoke(f, std::forward<Ts>(ts)..., &r)}; e != LIBULF_OK)
    throw libulf::ulf_error{e, libulf_last_error_string()};
  return r;
}

} // namespace

namespace libulf {

namespace mdu {

/**
 * Transfer speed type (MDU)
 */
enum class Speed : uint8_t {
  Fallback = 0u, ///< Fallback, always received
  Fast = 1u,     ///< Fast
  Medium = 2u,   ///< Medium
  Slow = 3u,     ///< Slow
  Default = 4u,  ///< Default
};

} // namespace mdu

struct MDU_EIN;   // Forward declare
struct DECUP_EIN; // Forward declare
struct SUSIV2;    // Forward declare

/**
 * ZPP file API group
 *
 * \details
 * This object is a fascade for the `::libulf_zpp_` api group, which manages
 * the `zpp` handle internally.
 *
 */
struct ZPP {
  /**
   * A single project CV
   */
  struct CV {
    uint16_t const address{}; ///< CV Address
    uint8_t const value{};    ///< CV Value
  };

  friend class MDU_EIN;
  friend class SUSIV2;
  friend class DECUP_EIN;

  /**
   * CTor
   *
   * \note Corresponds to `::libulf_zpp_read`
   *
   * \details
   * Reads the .zpp file at `path`
   *
   * \warning Since reading the file at path may fail, use of \ref ZPP::valid is
   * recommended.
   *
   * @param path The path to the .zpp file (must be accessible with full rights)
   */
  ZPP(std::filesystem::path path)
    : _zpp{libulf_zpp_read(path.string().data(), path.string().size())} {}

  /**
   * Move CTor
   *
   * \warning
   * Invalidates `source` and moves the handle to newly constructed ZPP
   *
   * \param source Source ZPP
   */
  ZPP(ZPP&& source) : _zpp{source._zpp} { source._zpp = nullptr; }

  ZPP() = delete;
  ZPP(ZPP const&) = delete;
  ZPP& operator=(const ZPP&) = delete;
  ZPP& operator=(ZPP&&) = delete;

  /**
   * DTor
   *
   * \note Corresponds to `::libulf_zpp_release`
   *
   * \details
   * Deletest the underlying handle
   *
   */
  ~ZPP() {
    if (_zpp) libulf_zpp_release(_zpp);
  }

  /**
   * Checks if the underlying handle exists (non-nullptr)
   *
   * \return true   Valid
   * \return false  Invalid
   */
  bool valid() const { return _zpp; }

  /**
   * Returns the number of flash blocks within the project
   *
   * \note Corresponds to `::libulf_zpp_blocks`
   *
   * \warning Crashes without handle ( \ref ZPP::valid )
   *
   * \return unsigned int Block count
   */
  unsigned int blockCount() const { return libulf_zpp_block_count(_zpp); }

  /**
   * Returns an iterable view on the projects CV list
   *
   * \return CV list
   */
  auto cvs() const {
    return std::views::iota(0uz, libulf_zpp_cv_count(_zpp)) |
           std::views::transform([this](unsigned int i) {
             return CV{.address = libulf_zpp_cv_address(_zpp, i),
                       .value = libulf_zpp_cv_value(_zpp, i)};
           });
  }

  /**
   * Returns the author of the project
   *
   * \note Corresponds to `::libulf_zpp_name`
   *
   * \warning Crashes without handle ( \ref ZPP::valid )
   *
   * \return std::string_view Name
   */
  std::string_view author() const { return {libulf_zpp_author(_zpp)}; }

  /**
   * Returns the email of the project author
   *
   * \note Corresponds to `::libulf_zpp_email`
   *
   * \warning Crashes without handle ( \ref ZPP::valid )
   *
   * \return std::string_view Email
   */
  std::string_view email() const { return {libulf_zpp_email(_zpp)}; }

private:
  /// Internal convenience cast
  explicit operator zpp_handle() { return _zpp; }

  zpp_handle _zpp; ///< Underlying handle
};

struct MDU_EIN;   // Forward declare
struct DECUP_EIN; // Forward declare

/**
 * ZPP file API group
 *
 * \details
 * This object is a fascade for the `::libulf_zsu_` api group, which manages
 * the `zsu` handle internally.
 *
 */
struct ZSU {
  struct Firmware {
    friend class ZSU;
    friend class MDU_EIN;
    friend class DECUP_EIN;

    /**
     * Returns the ID of the current firmware (matches the compatible decoder
     * ID)
     *
     * \note Corresponds to `::libulf_zsu_get_firmware_id`
     *
     * \return uint32_t Decoder ID
     */
    uint32_t id() const { return libulf_zsu_get_firmware_id(_zsu, _fwIndex); }

    /**
     * Returns the name of the current firmware
     *
     * \note Corresponds to `::libulf_zsu_get_firmware_name`
     *
     * \return std::string_view Decoder name
     */
    std::string_view name() const {
      return {libulf_zsu_get_firmware_name(_zsu, _fwIndex)};
    }

    /**
     * Returns the major version of the current firmware
     *
     * \note Corresponds to `::libulf_zsu_get_firmware_major_version`
     *
     * \return std::string_view Major version
     */
    std::string_view versionMajor() const {
      return {libulf_zsu_get_firmware_major_version(_zsu, _fwIndex)};
    }

    /**
     * Returns the minor version of the current firmware
     *
     * \note Corresponds to `::libulf_zsu_get_firmware_minor_version`
     *
     * \return std::string_view Minor version
     */
    std::string_view versionMinor() const {
      return {libulf_zsu_get_firmware_minor_version(_zsu, _fwIndex)};
    }

    /**
     * Returns the bootloader type of the current firmware (relevant for MX
     * only)
     *
     * \note Corresponds to `::libulf_zsu_get_firmware_type`
     *
     * \return int Bootloader type
     */
    int type() const { return libulf_zsu_get_firmware_type(_zsu, _fwIndex); }

    /**
     * Returns the number of flash blocks within the current firmware
     *
     * \note Corresponds to `::libulf_zsu_get_firmware_block_count`
     *
     * \return unsigned int Block count
     */
    unsigned int blockCount() const {
      return libulf_zsu_get_firmware_block_count(_zsu, _fwIndex);
    }

    /**
     * Returns the data of the current firmware as a span
     *
     * \note Corresponds to `::libulf_zsu_get_firmware_data` and
     * `::libulf_zsu_get_firmware_data_size` combined
     *
     * \return std::span data
     */
    std::span<uint8_t const> data() const {
      return {libulf_zsu_get_firmware_data(_zsu, _fwIndex),
              libulf_zsu_get_firmware_data_size(_zsu, _fwIndex)};
    }

  private:
    // Internal CTors
    Firmware(zsu_handle zsu, unsigned int fwIndex = 0uz)
      : _zsu{zsu}, _fwIndex{fwIndex} {}

    zsu_handle _zsu;         ///< Underlying ZSU handle
    unsigned int _fwIndex{}; ///< Firmware index
  };

  /**
   * CTor
   *
   * \note Corresponds to `::libulf_zsu_read`
   *
   * \warning Since reading the file at path may fail, use of \ref ZSU::valid
   * is recommended.
   *
   * \param path Path to ZSU
   */
  ZSU(std::filesystem::path path)
    : _zsu{libulf_zsu_read(path.string().data(), path.string().size())} {}

  ZSU() = delete;
  ZSU(ZSU const&) = delete;
  ZSU& operator=(const ZSU&) = delete;
  ZSU(ZSU&& source) : _zsu{source._zsu} { source._zsu = nullptr; }
  ZSU& operator=(ZSU&&) = delete;
  ~ZSU() {
    if (_zsu) libulf_zsu_release(_zsu);
  }

  /**
   * Checks if the underlying handle exists (non-nullptr)
   *
   * \return true   Valid
   * \return false  Not valid
   */
  bool valid() const { return _zsu; }

  auto firmwares() const {
    return std::views::iota(0uz, libulf_zsu_get_firmware_count(_zsu)) |
           std::views::transform(
             [this](unsigned int i) { return Firmware{_zsu, i}; });
  }

private:
  // Internal convenience cast operator
  explicit operator zsu_handle() { return _zsu; }

  zsu_handle _zsu; ///< Underlying handle
};

struct LibULF; // Forward declare

/**
 * The COM protocol API group
 *
 */
struct COM {
  friend class LibULF;

  // Delete all CTors.. Or just don't construct this manually
  COM() = delete;
  COM(const COM&) = delete;
  COM& operator=(const COM&) = delete;
  COM(COM&&) = delete;
  COM& operator=(COM&&) = delete;
  ~COM() = default;

  /**
   * Sends a PING command to the device
   *
   * \return std::string  Response string
   *
   * \throws ulf_error   If an error occured
   */
  std::string ping() {
    std::string r{};
    r.resize(128uz);
    size_t s{r.size()};
    if (auto const e{libulf_com_ping(_lib, r.data(), &s)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};

    r.resize(s);
    return r;
  }

  /**
   * Sends a RESET command to the device
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool reset() { return execute<bool>(libulf_com_reset, _lib); }

  /**
   * Sends a SUSIV2 command to the device
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool susiv2() { return execute<bool>(libulf_com_susiv2, _lib); }

  /**
   * Sends a MDU_EIN command to the device
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool mdu_ein() { return execute<bool>(libulf_com_mdu_ein, _lib); }

  /**
   * Sends a DECUP_EIN command to the device
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool decup_ein() { return execute<bool>(libulf_com_decup_ein, _lib); }

private:
  // Internal CTor
  COM(libulf_handle& lib) : _lib{lib} {}

  libulf_handle& _lib; ///< Underlying handle
};

/**
 * The SUSIV2 protocol API group
 *
 */
struct SUSIV2 {
  friend class LibULF;

  // Delete all CTors.. Or just don't construct this manually
  SUSIV2() = delete;
  SUSIV2(const SUSIV2&) = delete;
  SUSIV2& operator=(const SUSIV2&) = delete;
  SUSIV2(SUSIV2&&) = delete;
  SUSIV2& operator=(SUSIV2&&) = delete;
  ~SUSIV2() = default;

  /**
   * Reads a CV from the decoder
   *
   * \param cv    Cv address to read
   *
   * \return uint8_t      Value of the CV
   * \return std::nullopt No value received
   *
   * \throws ulf_error    If an error occurred
   */
  std::optional<uint8_t> cvRead(uint16_t cv) {
    if (auto const r{execute<int>(libulf_susiv2_cv_read, _lib, cv)}; r >= 0)
      return static_cast<uint8_t>(r);
    return std::nullopt;
  }

  /**
   * Writes a CV to the decoder
   *
   * \param cv    Cv address to write
   * \param value Cv value to write
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool cvWrite(uint16_t cv, uint8_t value) {
    return execute<bool>(libulf_susiv2_cv_write, _lib, cv, value);
  }

  /**
   * Erases the sound flash of the decoder
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppErase() { return execute<bool>(libulf_susiv2_zpp_erase, _lib); }

  /**
   * Writes a sound flash block to the decoder
   *
   * \param zpp     ZPP
   * \param index   Block index
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppWrite(ZPP& zpp, uint32_t index) {
    return execute<bool>(
      libulf_susiv2_zpp_write, _lib, static_cast<zpp_handle>(zpp), index);
  }

  /**
   * Requests decoder features
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool features() { return execute<bool>(libulf_susiv2_features, _lib); }

  /**
   * Requests the decoder to quit ZUSI mode
   *
   * \param reboot      Reboot decoder
   * \param cv8_reset   Cv8 reset (reload CVs from flash)
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool exit(bool reboot, bool cv8_reset) {
    return execute<bool>(libulf_susiv2_exit, _lib, reboot, cv8_reset);
  }

  /**
   * Checks, if the load code on the decoder is valid for the developer
   * code of the given ZPP
   *
   * \param zpp   ZPP
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppLcDcQuery(ZPP& zpp) {
    return execute<bool>(
      libulf_susiv2_zpp_lc_dc_query, _lib, static_cast<zpp_handle>(zpp));
  }

private:
  // Internal CTor
  SUSIV2(libulf_handle& lib) : _lib{lib} {}

  libulf_handle& _lib; ///< Underlying handle
};

/**
 * The MDU_EIN protocol API group
 *
 */
struct MDU_EIN {
  friend class LibULF;

  // Delete all CTors.. Or just don't construct this manually
  MDU_EIN() = delete;
  MDU_EIN(const MDU_EIN&) = delete;
  MDU_EIN& operator=(const MDU_EIN&) = delete;
  MDU_EIN(MDU_EIN&&) = delete;
  MDU_EIN& operator=(MDU_EIN&&) = delete;
  ~MDU_EIN() = default;

  /**
   * Commands all decoders to enter MDU mode (Update)
   *
   * \note Corresponds to `::libulf_mdu_ein_enter_mdu`
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool enterMDU() { return execute<bool>(libulf_mdu_ein_enter_mdu, _lib); }

  /**
   * Commands selected decoders to enter MDU mode (Update)
   *
   * \note Corresponds to `::libulf_mdu_ein_enter_dcc_zsu`
   *
   * \param id    Decoder ID
   * \param sh    Decoder serial number
   * \param done  `true` done with entry, `false` more [id,sn] pairs will follow
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool enterDCCZSU(uint32_t id = 0uz, uint32_t sn = 0uz, bool done = true) {
    return execute<bool>(libulf_mdu_ein_enter_dcc_zsu, _lib, id, sn, done);
  }

  /**
   * Commands selected decoders to enter MDU mode (SoundLoad)
   *
   * \note Corresponds to `::libulf_mdu_ein_enter_dcc_zpp`
   *
   * \param sn    Decoder ID
   * \param done  `true` done with entry, `false` more sn will follow
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool enterDCCZPP(uint32_t sn = 0uz, bool done = true) {
    return execute<bool>(libulf_mdu_ein_enter_dcc_zpp, _lib, sn, done);
  }

  /**
   * Pings decoder(-s)
   *
   * \note Corresponds to `::libulf_mdu_ein_ping`
   *
   * \param sn  Decoder serial number
   * \param id  Decoder ID
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool ping(uint32_t sn, uint32_t id) {
    return execute<bool>(libulf_mdu_ein_ping, _lib, sn, id);
  }

  /**
   * Configures transfer rate for decoder and USB device
   *
   * \note Corresponds to `::libulf_mdu_ein_config_transfer_rate`
   *
   * \note Both, the decoder speed and device speed will be updated. If the
   * update fails, device speed will be set to fallback.
   *
   * \note It is recommended to use this at least once after start, since else
   * the speed will be at default (which is slower than slow)
   *
   * \param speed   Speed to set
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool configTransferRate(mdu::Speed speed) {
    return execute<bool>(
      libulf_mdu_ein_config_transfer_rate, _lib, std::to_underlying(speed));
  }

  /**
   * Reads a CV from the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_cv_read`
   *
   * \param cv  Cv address to read
   *
   * \return int          Value of the CV
   *
   * \throws ulf_error    If an error occurred
   */
  int cvRead(uint16_t cv) {
    return execute<int>(libulf_mdu_ein_cv_read, _lib, cv);
  }

  /**
   * Writes a CV to the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_cv_write`
   *
   * \param cv    Cv address to write
   * \param value Cv value to write
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool cvWrite(uint16_t cv, uint8_t value) {
    return execute<bool>(libulf_mdu_ein_cv_write, _lib, cv, value);
  }

  /**
   * Checks if the decoder is busy
   *
   * \note Corresponds to `::libulf_mdu_ein_busy`
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool busy() { return execute<bool>(libulf_mdu_ein_busy, _lib); }

  /**
   * Checks if the given ZPP can fit into the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_zpp_valid_query`
   *
   * \param zpp   ZPP
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppValidQuery(ZPP& zpp) {
    return execute<bool>(
      libulf_mdu_ein_zpp_valid_query, _lib, static_cast<zpp_handle>(zpp));
  }

  /**
   * Checks, if the load code on the decoder is valid for the developer
   * code of the given ZPP
   *
   * \note Corresponds to `::libulf_mdu_ein_zpp_lc_dc_query`
   *
   * \param zpp   ZPP
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppLcDcQuery(ZPP& zpp) {
    return execute<bool>(
      libulf_mdu_ein_zpp_lc_dc_query, _lib, static_cast<zpp_handle>(zpp));
  }

  /**
   * Erases the sound flash of the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_zpp_erase`
   *
   * \param zpp ZPP
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppErase(ZPP& zpp) {
    return execute<bool>(
      libulf_mdu_ein_zpp_erase, _lib, static_cast<zpp_handle>(zpp));
  }

  /**
   * Writes a sound flash block to the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_zpp_update`
   *
   * \param zpp   ZPP
   * \param index Block index
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppUpdate(ZPP& zpp, uint32_t index) {
    return execute<bool>(
      libulf_mdu_ein_zpp_update, _lib, static_cast<zpp_handle>(zpp), index);
  }

  /**
   * Semantic end of the sound flash update
   *
   * \note Corresponds to `::libulf_mdu_ein_zpp_update_end`
   *
   * \param zpp ZPP
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppUpdateEnd(ZPP& zpp) {
    return execute<bool>(
      libulf_mdu_ein_zpp_update_end, _lib, static_cast<zpp_handle>(zpp));
  }

  /**
   * Command the decoder to exit MDU and reset
   *
   * \note Corresponds to `::libulf_mdu_ein_zpp_exit_reset`
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppExitReset() {
    return execute<bool>(libulf_mdu_ein_zpp_exit_reset, _lib);
  }

  /**
   * Initializes the Salsa20 encryption
   *
   * \note Corresponds to `::libulf_mdu_ein_zsu_salsa_20_iv`
   *
   * \param firmware  Firmware
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuSalsa20Iv(ZSU::Firmware& firmware) {
    return execute<bool>(
      libulf_mdu_ein_zsu_salsa20_iv, _lib, firmware._zsu, firmware._fwIndex);
  }

  /**
   * Erases the firmware flash of the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_zsu_erase`
   *
   * \param firmware  Firmware
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuErase(ZSU::Firmware& firmware) {
    return execute<bool>(
      libulf_mdu_ein_zsu_erase, _lib, firmware._zsu, firmware._fwIndex);
  }

  /**
   * Writes firmware flash block to the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_zsu_update`
   *
   * \param firmware  Firmware
   * \param index     Block index
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuUpdate(ZSU::Firmware& firmware, uint32_t index) {
    return execute<bool>(
      libulf_mdu_ein_zsu_update, _lib, firmware._zsu, firmware._fwIndex, index);
  }

  /**
   * Starts the firmware flash verification (CRC32)
   *
   * \note Corresponds to `::libulf_mdu_ein_zsu_crc32_start`
   *
   * \param firmware  Firmware
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuCrc32Start(ZSU::Firmware& firmware) {
    return execute<bool>(
      libulf_mdu_ein_zsu_crc32_start, _lib, firmware._zsu, firmware._fwIndex);
  }

  /**
   * Checks the result of the firmware flash verification
   *
   * \note Corresponds to `::libulf_mdu_ein_zsu_crc32_result`
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuCrc32Result() {
    return execute<bool>(libulf_mdu_ein_zsu_crc32_result, _lib);
  }

  /**
   * Checks the result of the firmware flash verification and commands the
   * decoder to leave MDU mode
   *
   * \note Corresponds to `::libulf_mdu_ein_resul_zsu_crc32_result_exit`
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuCrc32ResultExit() {
    return execute<bool>(libulf_mdu_ein_zsu_crc32_result_exit, _lib);
  }

private:
  // Internal CTor
  MDU_EIN(libulf_handle& lib) : _lib{lib} {}

  libulf_handle& _lib; ///< Underlying handle
};

struct DECUP_EIN {
  friend class LibULF;

  /**
   * Resets a decoder to bootloader in update mode
   *
   * \note Corresponds to `::libulf_decup_ein_enter_zsu`
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool enterZSU() { return execute<bool>(libulf_decup_ein_enter_zsu, _lib); }

  /**
   * Resets a decoder to bootloader in soundload mode
   *
   * \note Corresponds to `::libulf_decup_ein_enter_zsu`
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool enterZPP() { return execute<bool>(libulf_decup_ein_enter_zpp, _lib); }

  /**
   * Transmits a decoder id
   *
   * \note Corresponds to `::libulf_decup_ein_zsu_decoder_id`
   *
   * \param id Decoder Id
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuDecoderId(uint8_t id) {
    return execute<bool>(libulf_decup_ein_zsu_decoder_id, _lib, id);
  }

  /**
   * Transmits the update page count
   *
   * \note Corresponds to `::libulf_decup_ein_zsu_page_count`
   *
   * \param firmware Firmware
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuPageCount(ZSU::Firmware& firmware) {
    return execute<bool>(
      libulf_decup_ein_zsu_page_count, _lib, firmware._zsu, firmware._fwIndex);
  }

  /**
   * Transmits the update security bytes
   *
   * \note Corresponds to `::libulf_decup_ein_zsu_security_bytes`
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuSecurityBytes() {
    return execute<bool>(libulf_decup_ein_zsu_security_bytes, _lib);
  }

  /**
   * Transmits an update block
   *
   * \note Corresponds to `::libulf_decup_ein_zsu_blocks`
   *
   * \param firmware  Firmware
   * \param index     Block index
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuBlocks(ZSU::Firmware& firmware, std::size_t index) {
    return execute<bool>(libulf_decup_ein_zsu_blocks,
                         _lib,
                         firmware._zsu,
                         firmware._fwIndex,
                         index);
  }

  /**
   * Reads a sigle cv
   *
   * \param cv    Cv address
   *
   * \note Corresponds to `::libulf_decup_ein_cv_read`
   *
   * \return int          Read cv value
   *
   * \throws ulf_error    If an error occurred
   */
  int cvRead(uint16_t cv) {
    return execute<int>(libulf_decup_ein_cv_read, _lib, cv);
  }

  /**
   * Writes a single cv
   *
   * \note Corresponds to `::libulf_decup_ein_cv_write`
   *
   * \param cv      Cv address
   * \param value   Cv value
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool cvWrite(uint16_t cv, uint8_t value) {
    return execute<bool>(libulf_decup_ein_cv_write, _lib, cv, value);
  }

  /**
   * Reads the decoder id
   *
   * \note Corresponds to `::libulf_decup_ein_zpp_decoder_id`
   *
   * \return int          Read cv value
   *
   * \throws ulf_error    If an error occurred
   */
  int zppDecoderId() {
    return execute<int>(libulf_decup_ein_zpp_decoder_id, _lib);
  }

  /**
   * Erases the decoder sound flash
   *
   * \note Corresponds to `::libulf_decup_ein_zpp_erase_flash`
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppEraseFlash() {
    return execute<bool>(libulf_decup_ein_zpp_erase_flash, _lib);
  }

  /**
   * Writes a block to the decoder sound flash
   *
   * \note Corresponds to `::libulf_decup_ein_zpp_write_flash`
   *
   * \param zpp           ZPP project
   * \param index         ZPP block index
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppWriteFlash(ZPP& zpp, size_t index) {
    return execute<bool>(libulf_decup_ein_zpp_write_flash,
                         _lib,
                         static_cast<zpp_handle>(zpp),
                         index);
  }

private:
  DECUP_EIN(libulf_handle& lib) : _lib{lib} {}

  libulf_handle& _lib; ///< Underlying handle
};

/**
 * LibULF library wrapper
 *
 * \details Wraps libulf behind a class interface. As a limitation, since the
 * backend is dependent on a handle, an instance of this class can only be
 * moved.
 *
 */
struct LibULF {
  LibULF() : _lib{libulf_create()} {}
  LibULF(LibULF const&) = delete;
  LibULF& operator=(LibULF const&) = delete;

  LibULF(LibULF&& source) : _lib{source._lib} { source._lib = nullptr; }
  LibULF& operator=(LibULF&& source) {
    _lib = source._lib;
    source._lib = nullptr;
    return *this;
  }
  ~LibULF() { libulf_destroy(_lib); }

  /**
   * Initializes the USB layer
   *
   * \note Corresponds to `::libulf_init`
   *
   * \return int
   * \retval Any error occurred
   */
  Error init() { return static_cast<Error>(libulf_init(_lib)); }

  /**
   * Opens the first USB device mathing the given identifiers
   *
   * \note Corresponds to `::libulf_open`
   *
   * \param vid VID
   * \param pid PID
   *
   * \return int  Any error occurred
   */
  Error open(uint16_t vid, uint16_t pid) {
    return static_cast<Error>(libulf_open(_lib, vid, pid));
  }

  /**
   * Opens a the given USB device via its file descriptor
   *
   * \note Corresponds to `::libulf_openFd`
   *
   * \warning This exists only for the libusb backend.
   *
   * \param Fd  Filedescriptor
   *
   * \return int Any error occurred
   */
  Error openFd(int Fd) { return static_cast<Error>(libulf_openFd(_lib, Fd)); }

  /**
   * Closes the open USB device
   *
   * \note Corresponds to `::libulf_close`
   */
  Error close() { return static_cast<Error>(libulf_close(_lib)); }

  /**
   * Sets the USB RTS line state
   *
   * \note Corresponds to `::libulf_set_rts`
   *
   * \param enable if `true` enable, disable otherwise
   *
   * \return Error
   */
  Error setRTS(bool enable) {
    return static_cast<Error>(libulf_set_rts(_lib, enable));
  }

  /**
   * Sets the USB DTR line state
   *
   * \note Corresponds to `::libulf_set_dtr`
   *
   * \param enable if `true` enable, disable otherwise
   *
   * \return Error
   */
  Error setDTR(bool enable) {
    return static_cast<Error>(libulf_set_dtr(_lib, enable));
  }

  /**
   * Returns the COM API interface group
   *
   * \return COM&
   */
  COM& com() { return _com; }

  /**
   * Returns the SUSIV2 API interface group
   *
   * \return SUSIV2&
   */
  SUSIV2& susiv2() { return _susiv2; }

  /**
   * Returns the MDU_EIN API interface group
   *
   * \return MDU_EIN&
   */
  MDU_EIN& mdu_ein() { return _mdu_ein; }

  DECUP_EIN& decup_ein() { return _decup_ein; }

private:
  libulf_handle _lib; ///< Underlying handle

  COM _com{_lib};             ///< COM interface
  SUSIV2 _susiv2{_lib};       ///< SUSIV2 interface
  MDU_EIN _mdu_ein{_lib};     ///< MDU_EIN interface
  DECUP_EIN _decup_ein{_lib}; ///< DECUP_EIN interface
};

} // namespace libulf
