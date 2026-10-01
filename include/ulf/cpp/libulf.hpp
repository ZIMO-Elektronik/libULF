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
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include "error.hpp"
#include "error2string.hpp"
#include "ulf/c/libulf.h"
#include "ulf_error.hpp"

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

struct MDU_EIN; // Forward declare
struct SUSIV2;  // Forward declare

/**
 * ZPP file API group
 *
 * \details
 * This object is a fascade for the `::libulf_zpp_` api group, which manages
 * the `zpp` handle internally.
 *
 */
struct ZPP {
  friend class MDU_EIN;
  friend class SUSIV2;

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
  unsigned int blocks() const { return libulf_zpp_blocks(_zpp); }

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

struct MDU_EIN; // Forward declare

/**
 * ZPP file API group
 *
 * \details
 * This object is a fascade for the `::libulf_zsu_` api group, which manages
 * the `zsu` handle internally.
 *
 */
struct ZSU {
  /**
   * Firmware iterator wrapper
   *
   * \details Usually, this class is used to iterate over all firmwares and
   * select / search a decoder by pinging each id.
   *
   * \details This is the poor mans approach to mapping C++ iterators to C and
   * then again to C++. Which means it is neither perfect, nor good. But it
   * works and its better than throwing around a raw handle.
   *
   * \note To make this stl compliant, we'd need to create a type `Firmware` and
   * set it as a base type. However, then every dereferencing would create a
   * copy of `_zsu` and `_fwIndex`.
   *
   * \warning When a firmware is chosen, the Iterator shouldn't be incremented
   * or decremented. This WILL move to a new firmware which can't be checked and
   * may or may not destroy the decoder as a result. This shoud be
   * self-explainatory but who knows who will use this library.
   *
   */
  struct FirmwareIterator {
    friend class ZSU;
    friend class MDU_EIN;

    // Construct/copy/destroy
    FirmwareIterator() = delete;
    FirmwareIterator(FirmwareIterator const&) = default;
    FirmwareIterator& operator=(FirmwareIterator const&) = default;
    FirmwareIterator(FirmwareIterator&& source)
      : _zsu{source._zsu}, _fwIndex{source._fwIndex} {
      source._zsu = nullptr;
    }
    FirmwareIterator& operator=(FirmwareIterator&& source) {
      _zsu = source._zsu;
      _fwIndex = source._fwIndex;
      source._zsu = nullptr;
      return *this;
    }
    ~FirmwareIterator() = default;

    /**
     * Pre-increment
     *
     * \return FirmwareIterator&
     */
    FirmwareIterator& operator++() {
      _fwIndex++;
      return *this;
    }

    /**
     * Post-increment
     *
     * \return FirmwareIterator
     */
    FirmwareIterator operator++(int) {
      auto retval{*this};
      _fwIndex++;
      return retval;
    }

    /**
     * Pre-decrement
     *
     * \return FirmwareIterator&
     */
    FirmwareIterator& operator--() {
      _fwIndex--;
      return *this;
    }

    /**
     * Post-incement
     *
     * \return FirmwareIterator
     */
    FirmwareIterator operator--(int) {
      auto retval{*this};
      _fwIndex--;
      return retval;
    }

    /**
     * Comparison
     *
     * \param rhs
     * \return true   Equal
     * \return false  Not equal
     */
    bool operator==(FirmwareIterator const& rhs) const {
      return _zsu == rhs._zsu && _fwIndex == rhs._fwIndex;
    }

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
    FirmwareIterator(zsu_handle zsu, unsigned int fwIndex = 0uz)
      : _zsu{zsu}, _fwIndex{fwIndex} {}

    zsu_handle _zsu;         ///< Underlying ZSU handle
    unsigned int _fwIndex{}; ///< Firmware index
  };

  using iterator = FirmwareIterator;
  using const_iterator = FirmwareIterator;

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

  /**
   * Begin
   *
   * \return iterator Begin iterator
   */
  iterator begin() const { return FirmwareIterator{_zsu}; }

  /**
   * End
   *
   * \return iterator End iterator
   */
  iterator end() const {
    return FirmwareIterator{_zsu, libulf_zsu_get_firmware_count(_zsu)};
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
  bool reset() {
    bool r{};
    if (auto const e{libulf_com_reset(_lib, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

  /**
   * Sends a SUSIV2 command to the device
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool susiv2() {
    bool r{};
    if (auto const e{libulf_com_susiv2(_lib, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

  /**
   * Sends a MDU_EIN command to the device
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool mdu_ein() {
    bool r{};
    if (auto const e{libulf_com_mdu_ein(_lib, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

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
    int r{};
    if (auto const e{libulf_susiv2_cv_read(_lib, cv, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};

    if (r >= 0) return static_cast<uint8_t>(r);
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
    bool r{};
    if (auto const e{libulf_susiv2_cv_write(_lib, cv, value, &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

  /**
   * Erases the sound flash of the decoder
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zppErase() {
    bool r{};
    if (auto const e{libulf_susiv2_zpp_erase(_lib, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

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
    bool r{};
    if (auto const e{libulf_susiv2_zpp_write(
          _lib, static_cast<zpp_handle>(zpp), index, &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

  /**
   * Requests decoder features
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool features() {
    bool r{};
    if (auto const e{libulf_susiv2_features(_lib, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

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
    bool r{};
    if (auto const e{libulf_susiv2_exit(
          _lib, static_cast<bool>(reboot), static_cast<bool>(cv8_reset), &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{libulf_susiv2_zpp_lc_dc_query(
          _lib, static_cast<zpp_handle>(zpp), &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
  bool enterMDU() {
    bool r{};
    if (auto const e{libulf_mdu_ein_enter_mdu(_lib, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

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
    bool r{};
    if (auto const e{libulf_mdu_ein_enter_dcc_zsu(
          _lib, id, sn, static_cast<bool>(done), &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{
          libulf_mdu_ein_enter_dcc_zpp(_lib, sn, static_cast<bool>(done), &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{libulf_mdu_ein_ping(_lib, sn, id, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{libulf_mdu_ein_config_transfer_rate(
          _lib, std::to_underlying(speed), &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

  /**
   * Reads a CV from the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_cv_read`
   *
   * \param cv  Cv address to read
   *
   * \return uint8_t      Value of the CV
   *
   * \throws ulf_error    If an error occurred
   */
  uint8_t cvRead(uint16_t cv) {
    int r{};
    if (auto const e{libulf_mdu_ein_cv_read(_lib, cv, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return static_cast<uint8_t>(r);
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
    bool r{};
    if (auto const e{libulf_mdu_ein_cv_write(_lib, cv, value, &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
  bool busy() {
    bool r{};
    if (auto const e{libulf_mdu_ein_busy(_lib, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

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
    bool r{};
    if (auto const e{libulf_mdu_ein_zpp_valid_query(
          _lib, static_cast<zpp_handle>(zpp), &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{libulf_mdu_ein_zpp_lc_dc_query(
          _lib, static_cast<zpp_handle>(zpp), &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{
          libulf_mdu_ein_zpp_erase(_lib, static_cast<zpp_handle>(zpp), &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{libulf_mdu_ein_zpp_update(
          _lib, static_cast<zpp_handle>(zpp), index, &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{libulf_mdu_ein_zpp_update_end(
          _lib, static_cast<zpp_handle>(zpp), &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{libulf_mdu_ein_zpp_exit_reset(_lib, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

  /**
   * Initializes the Salsa20 encryption
   *
   * \note Corresponds to `::libulf_mdu_ein_zsu_salsa_20_iv`
   *
   * \param firmware  FirmwareIterator
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuSalsa20Iv(ZSU::FirmwareIterator& firmware) {
    bool r{};
    if (auto const e{libulf_mdu_ein_zsu_salsa20_iv(
          _lib, firmware._zsu, firmware._fwIndex, &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

  /**
   * Erases the firmware flash of the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_zsu_erase`
   *
   * \param firmware  FirmwareIterator
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuErase(ZSU::FirmwareIterator& firmware) {
    bool r{};
    if (auto const e{
          libulf_mdu_ein_zsu_erase(_lib, firmware._zsu, firmware._fwIndex, &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

  /**
   * Writes firmware flash block to the decoder
   *
   * \note Corresponds to `::libulf_mdu_ein_zsu_update`
   *
   * \param firmware  FirmwareIterator
   * \param index     Block index
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuUpdate(ZSU::FirmwareIterator& firmware, uint32_t index) {
    bool r{};
    if (auto const e{libulf_mdu_ein_zsu_update(
          _lib, firmware._zsu, firmware._fwIndex, index, &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

  /**
   * Starts the firmware flash verification (CRC32)
   *
   * \note Corresponds to `::libulf_mdu_ein_zsu_crc32_start`
   *
   * \param firmware  FirmwareIterator
   *
   * \return bool         `true` if successful, `false` else
   *
   * \throws ulf_error    If an error occurred
   */
  bool zsuCrc32Start(ZSU::FirmwareIterator& firmware) {
    bool r{};
    if (auto const e{libulf_mdu_ein_zsu_crc32_start(
          _lib, firmware._zsu, firmware._fwIndex, &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{libulf_mdu_ein_zsu_crc32_result(_lib, &r)}; e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
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
    bool r{};
    if (auto const e{libulf_mdu_ein_zsu_crc32_result_exit(_lib, &r)};
        e != LIBULF_OK)
      throw ulf_error{e, libulf_last_error_string()};
    return r;
  }

private:
  // Internal CTor
  MDU_EIN(libulf_handle& lib) : _lib{lib} {}

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
  LibULF& operator=(LibULF&& source) = delete;
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

private:
  libulf_handle _lib; ///< Underlying handle

  COM _com{_lib};         ///< COM interface
  SUSIV2 _susiv2{_lib};   ///< SUSIV2 interface
  MDU_EIN _mdu_ein{_lib}; ///< MDU_EIN interface
};

} // namespace libulf
