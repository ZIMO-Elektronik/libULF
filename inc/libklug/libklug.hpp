/**
 * Libklug C++ wrapper
 *
 * This is a simple wrapper header, that wraps the LibKLUG C API in flat C++
 * classes with. The hierarchy remains the same.
 *
 * Pretty much every class here can either be constructed with their own default
 * argument, or move constructed. Anything else may (or will) result in severe
 * state inconsistencies, crashes, memory leaks and or world destruction. Things
 * that happen when you do thins which are not recommended.
 *
 * And yes, moving an object and then using the moved object WILL crash the APP.
 *
 * \file    inc/libklug/libklug.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 *
 * \todo Most things that could be `const` are not. To change this, the C API
 * must be changed as well
 */

#pragma once

#include <cassert>
#include <filesystem>
#include <functional>
#include <optional>
#include <string_view>
#include <utility>
#include "libklug.h"
#include "result/dispatch.hpp"

namespace libklug {

namespace mdu {

/**
 * MDU Transfer speed
 *
 * \note Software updates are only safe up to \ref Speed::Slow
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
 * ZPP File wrapper
 *
 * \details Constructible from a path or by move.
 *
 * \warning Moving causes the underlying handle to become null. This can be
 * checked using \ref ZPP::valid.
 *
 */
struct ZPP {
  friend class MDU_EIN;
  friend class SUSIV2;

  /**
   * CTor
   *
   * \warning Since reading the file at path may fail, use of \ref ZPP::valid is
   * recommended.
   *
   * \param path Path to ZPP
   */
  ZPP(std::filesystem::path path)
    : _zpp{libklug_zpp_read(path.string().data(), path.string().size())} {}

  ZPP() = delete;
  ZPP(ZPP const&) = delete;
  ZPP& operator=(const ZPP&) = delete;
  ZPP(ZPP&&) = delete;
  ZPP& operator=(ZPP&&) = delete;
  ZPP(ZPP&& source) : _zpp{source._zpp} { source._zpp = nullptr; }
  ~ZPP() {
    if (_zpp) libklug_zpp_release(_zpp);
  }

  /**
   * Check if the underlying handle exists (non-nullptr)
   *
   * \return true   Valid
   * \return false  Invalid
   */
  bool valid() { return _zpp; }

  /**
   * Get the total count of flash blocks available
   *
   * \warning Crashes without handle ( \ref ZPP::valid )
   *
   * \return unsigned int Block count
   */
  unsigned int blocks() { return libklug_zpp_blocks(_zpp); }

  /**
   * Get the Author name
   *
   * \warning Crashes without handle ( \ref ZPP::valid )
   *
   * \return std::string_view Name
   */
  std::string_view author() { return {libklug_zpp_author(_zpp)}; }

  /**
   * Get the Email of the Author
   *
   * \warning Crashes without handle ( \ref ZPP::valid )
   *
   * \return std::string_view Email
   */
  std::string_view email() { return {libklug_zpp_email(_zpp)}; }

private:
  /// Internal convenience cast
  explicit operator zpp_handle() { return _zpp; }

  zpp_handle _zpp; ///< Underlying handle
};

struct MDU_EIN; // Forward declare

/**
 * ZSU File wrapper
 *
 * \details Constructible from a path or by move.
 *
 * \warning Moving causes the underlying handle to become null. This can be
 * checked using \ref ZPP::valid.
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
   * \warning When a firmware is chosen, the Iterator shouldn't be incremented
   * or decremented. This WILL move to a new firmware which can't be checked and
   * may or may not destroy the decoder as a result. This shoud be
   * self-explainatory but who knows who will use this library.
   *
   */
  struct FirmwareIterator {
    friend class ZSU;
    friend class MDU_EIN;

    // Delete all CTors.. Or just don't construct this manually
    FirmwareIterator() = delete;
    FirmwareIterator(FirmwareIterator const&) = delete;
    FirmwareIterator& operator=(FirmwareIterator const&) = delete;
    FirmwareIterator(FirmwareIterator&& source) : _fwIt{source._fwIt} {
      source._fwIt = nullptr;
    }
    FirmwareIterator& operator=(FirmwareIterator&&) = delete;
    ~FirmwareIterator() {
      if (_fwIt) libklug_zsu_destroy_firmware_iterator(_fwIt);
    }

    /**
     * Pre-increment
     *
     * \note By desing, there is no post-increment
     *
     * \return FirmwareIterator&
     */
    FirmwareIterator& operator++() {
      libklug_zsu_firmware_iterator_next(_fwIt);
      return *this;
    }

    /**
     * Pre-decrement
     *
     * \note By desing, there is no post-decrement
     *
     * \return FirmwareIterator&
     */
    FirmwareIterator& operator--() {
      libklug_zsu_firmware_iterator_previous(_fwIt);
      return *this;
    }

    /**
     * Comparison
     *
     * \param rhs
     * \return true   Equal
     * \return false  Not equal
     */
    bool operator==(FirmwareIterator const& rhs) const {
      return libklug_zsu_firmware_iterator_equals(
        static_cast<firmware_iterator_handle const>(*this),
        static_cast<firmware_iterator_handle const>(rhs));
    }

    FirmwareIterator operator++(int) = delete;
    FirmwareIterator operator--(int) = delete;

    /**
     * Get the ID of the decoder matching the current firmware
     *
     * \return uint32_t Decoder ID
     */
    uint32_t id() { return libklug_zsu_firmware_iterator_get_id(_fwIt); }

    /**
     * Get the name of the decoder matching the current firmware
     *
     * \return std::string_view Decoder name
     */
    std::string_view name() {
      return {libklug_zsu_firmware_iterator_get_name(_fwIt)};
    }

    /**
     * Get the major version of the current firmware
     *
     * \return std::string_view Major version
     */
    std::string_view versionMajor() {
      return {libklug_zsu_firmware_iterator_get_version_major(_fwIt)};
    }

    /**
     * Get the minor version of the current firmware
     *
     * \return std::string_view Minor version
     */
    std::string_view versionMinor() {
      return {libklug_zsu_firmware_iterator_get_version_minor(_fwIt)};
    }

    /**
     * Get the firmware type
     *
     * \details AFAIK this means the bootloader? Which means legacy from the
     * MX-Generation
     *
     * \return int Bootloader type
     */
    int type() { return libklug_zsu_firmware_iterator_get_type(_fwIt); }

    /**
     * Get the total flash block count of the current firmware
     *
     * \return unsigned int Block cound
     */
    unsigned int blocks() {
      return libklug_zsu_firmware_iterator_get_blocks(_fwIt);
    }

  private:
    // Internal CTors
    FirmwareIterator(zsu_handle zsu)
      : _fwIt{libklug_zsu_firmware_iterator_create_begin(zsu)} {}
    FirmwareIterator(zsu_handle zsu, bool)
      : _fwIt{libklug_zsu_firmware_iterator_create_end(zsu)} {}

    // Internal convenience cast operators
    explicit operator firmware_iterator_handle() { return _fwIt; }
    explicit operator firmware_iterator_handle const() const { return _fwIt; }

    firmware_iterator_handle _fwIt; ///< Underlying iterator handle
  };

  using iterator = FirmwareIterator;

  /**
   * CTor
   *
   * \warning Since reading the file at path may fail, use of \ref ZSU::valid is
   * recommended.
   *
   * \param path Path to ZSU
   */
  ZSU(std::filesystem::path path)
    : _zsu{libklug_zsu_read(path.string().data(), path.string().size())} {}

  ZSU() = delete;
  ZSU(ZSU const&) = delete;
  ZSU& operator=(const ZSU&) = delete;
  ZSU(ZSU&&) = delete;
  ZSU& operator=(ZSU&&) = delete;
  ZSU(ZSU&& source) : _zsu{source._zsu} { source._zsu = nullptr; }
  ~ZSU() {
    if (_zsu) libklug_zsu_release(_zsu);
  }

  /**
   * Checks if the underlying handle exists (non-nullptr)
   *
   * \return true   Valid
   * \return false  Not valid
   */
  bool valid() { return _zsu; }

  /**
   * Begin
   *
   * \return iterator Begin iterator
   */
  iterator begin() { return FirmwareIterator{_zsu}; }

  /**
   * End
   *
   * \return iterator End iterator
   */
  iterator end() { return FirmwareIterator{_zsu, bool{}}; }

private:
  // Internal convenience cast operator
  explicit operator zsu_handle() { return _zsu; }

  zsu_handle _zsu; ///< Underlying handle
};

struct LibKLUG; // Forward declare

/**
 * COM component wrapper
 *
 * \details Interface for ops following the ULF_COM protocol
 *
 * \note This class exists to avoid polluting the namespace like the C API
 * does.
 *
 */
struct COM {
  friend class LibKLUG;

  // Delete all CTors.. Or just don't construct this manually
  COM() = delete;
  COM(const COM&) = delete;
  COM& operator=(const COM&) = delete;
  COM(COM&&) = delete;
  COM& operator=(COM&&) = delete;
  ~COM() = default;

  /**
   * Start PING transmission
   *
   * \details At some point after start, this will produce a \ref res::String
   * result containing the response string.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool ping() { return libklug_com_ping(_lib) > 0; }

  /**
   * Start RESET transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool reset() { return libklug_com_reset(_lib) > 0; }

  /**
   * Start SUSIV2 transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool susiv2() { return libklug_com_susiv2(_lib) > 0; }

  /**
   * Start MDU_EIN transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool mdu_ein() { return libklug_com_mdu_ein(_lib) > 0; }

private:
  // Internal CTor
  COM(libklug_handle& lib) : _lib{lib} {}

  libklug_handle& _lib; ///< Underlying handle
};

/**
 * SUSIV2 component wrapper
 *
 * \details Interface for ops following the ULF_SUSIV2 protocol
 *
 * \details This exits to avoid polluting the namespace like the C API does
 *
 */
struct SUSIV2 {
  friend class LibKLUG;

  // Delete all CTors.. Or just don't construct this manually
  SUSIV2() = delete;
  SUSIV2(const SUSIV2&) = delete;
  SUSIV2& operator=(const SUSIV2&) = delete;
  SUSIV2(SUSIV2&&) = delete;
  SUSIV2& operator=(SUSIV2&&) = delete;
  ~SUSIV2() = default;

  /**
   * Starts a Cv read transmission
   *
   * \details At some point after start, this will produce a \ref res::Cv
   * result.
   *
   * \param cv    Cv address to read
   *
   * \return true   Started
   * \return false  Busy
   */
  bool cvRead(uint16_t cv) { return libklug_susiv2_cv_read(_lib, cv) > 0; }

  /**
   * Starts a Cv write transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param cv    Cv address to write
   * \param value Cv value to write
   *
   * \return true   Started
   * \return false  Busy
   */
  bool cvWrite(uint16_t cv, uint8_t value) {
    return libklug_susiv2_cv_write(_lib, cv, value) > 0;
  }

  /**
   * Starts a ZPP erase transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zppErase() { return libklug_susiv2_zpp_erase(_lib) > 0; }

  /**
   * Starts a ZPP write transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param zpp
   * \param index
   * \return true   Started
   * \return false  Busy
   */
  bool zppWrite(ZPP& zpp, uint32_t index) {
    return libklug_susiv2_zpp_write(_lib, static_cast<zpp_handle>(zpp), index) >
           0;
  }

  /**
   * Starts a Features transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool features() { return libklug_susiv2_features(_lib) > 0; }

  /**
   * Starts an Exit transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param reboot      Reboot decoder
   * \param cv8_reset   Cv8 reset (reload CVs from flash)
   *
   * \return true   Started
   * \return false  Busy
   */
  bool exit(bool reboot, bool cv8_reset) {
    return libklug_susiv2_exit(_lib, reboot, cv8_reset) > 0;
  }

  /**
   * Starts a ZPP LC DC Query
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param zpp   ZPP
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zppLcDcQuery(ZPP& zpp) {
    return libklug_susiv2_zpp_lc_dc_query(_lib, static_cast<zpp_handle>(zpp)) >
           0;
  }

private:
  // Internal CTor
  SUSIV2(libklug_handle& lib) : _lib{lib} {}

  libklug_handle& _lib; ///< Underlying handle
};

/**
 * MDU_EIN component wrapper
 *
 * \details Interface for ops following the ULF_MDU_EIN protocol
 *
 * \details This exits to avoid polluting the namespace like the C API does
 *
 */
struct MDU_EIN {
  friend class LibKLUG;

  // Delete all CTors.. Or just don't construct this manually
  MDU_EIN() = delete;
  MDU_EIN(const MDU_EIN&) = delete;
  MDU_EIN& operator=(const MDU_EIN&) = delete;
  MDU_EIN(MDU_EIN&&) = delete;
  MDU_EIN& operator=(MDU_EIN&&) = delete;
  ~MDU_EIN() = default;

  /**
   * Starts a MDU (Powercycle) entry transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool enterMDU() { return libklug_mdu_ein_enter_mdu(_lib); }

  /**
   * Starts a DCC ZSU entry transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \todo This needs be able to send at least a pair of sn and id. Currently,
   * the lib auto-sends zero for both.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool enterDCCZSU() { return libklug_mdu_ein_enter_dcc_zsu(_lib); }

  /**
   * Starts a DCC ZPP entry transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool enterDCCZPP() { return libklug_mdu_ein_enter_dcc_zpp(_lib); }

  /**
   * Starts a ping transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param sn  Decoder serial number
   * \param id  Decoder ID
   *
   * \return true   Started
   * \return false  Busy
   */
  bool ping(uint32_t sn, uint32_t id) {
    return libklug_mdu_ein_ping(_lib, sn, id);
  }

  /**
   * Starts a config transfer rate transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \note Both, the decoder speed and device speed will be updated. If the
   * update fails, device speed will be set to fallback.
   *
   * \note It is recommended to use this at least once after start, since else
   * the speed will be at default (which is slower than slow)
   *
   * \param speed   Speed to set
   *
   * \return true   Started
   * \return false  Busy
   */
  bool configTransferRate(mdu::Speed speed) {
    return libklug_mdu_ein_config_transfer_rate(_lib,
                                                std::to_underlying(speed));
  }

  /**
   * Starts a Cv read transmission
   *
   * \details At some point after start, this will produce a \ref res::Cv
   * result.
   *
   * \param cv  Cv address to read
   *
   * \return true   Started
   * \return false  Busy
   */
  bool cvRead(uint16_t cv) { return libklug_mdu_ein_cv_read(_lib, cv); }

  /**
   * Starts a Cv write transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param cv    Cv address to write
   * \param value Cv value to write
   *
   * \return true   Started
   * \return false  Busy
   */
  bool cvWrite(uint16_t cv, uint8_t value) {
    return libklug_mdu_ein_cv_write(_lib, cv, value);
  }

  /**
   * Starts a busy transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool busy() { return libklug_mdu_ein_busy(_lib); }

  /**
   * Starts a ZPP valid query transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param zpp   ZPP
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zppValidQuery(ZPP& zpp) {
    return libklug_mdu_ein_zpp_valid_query(_lib, static_cast<zpp_handle>(zpp));
  }

  /**
   * Starts a ZPP LC DC query transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param zpp   ZPP
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zppLcDcQuery(ZPP& zpp) {
    return libklug_mdu_ein_zpp_lc_dc_query(_lib, static_cast<zpp_handle>(zpp));
  }

  /**
   * Starts a ZPP erase transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param zpp ZPP
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zppErase(ZPP& zpp) {
    return libklug_mdu_ein_zpp_erase(_lib, static_cast<zpp_handle>(zpp));
  }

  /**
   * Starts a ZPP update transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param zpp   ZPP
   * \param index Block index
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zppUpdate(ZPP& zpp, uint32_t index) {
    return libklug_mdu_ein_zpp_update(
      _lib, static_cast<zpp_handle>(zpp), index);
  }

  /**
   * Starts a ZPP update end transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param zpp ZPP
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zppUpdateEnd(ZPP& zpp) {
    return libklug_mdu_ein_zpp_update_end(_lib, static_cast<zpp_handle>(zpp));
  }

  /**
   * Starts a ZPP exit reset transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zppExitReset() { return libklug_mdu_ein_zpp_exit_reset(_lib); }

  /**
   * Starts a ZSU salsa20 init transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param firmware  FirmwareIterator
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zsuSalsa20Iv(ZSU::FirmwareIterator& firmware) {
    return libklug_mdu_ein_zsu_salsa20_iv(
      _lib, static_cast<firmware_iterator_handle>(firmware));
  }

  /**
   * Starts a ZSU erase transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param firmware  FirmwareIterator
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zsuErase(ZSU::FirmwareIterator& firmware) {
    return libklug_mdu_ein_zsu_erase(
      _lib, static_cast<firmware_iterator_handle>(firmware));
  }

  /**
   * Starts a ZSU update transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param firmware  FirmwareIterator
   * \param index     Block index
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zsuUpdate(ZSU::FirmwareIterator& firmware, uint32_t index) {
    return libklug_mdu_ein_zsu_update(
      _lib, static_cast<firmware_iterator_handle>(firmware), index);
  }

  /**
   * Starts a ZSU crc32 start transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \param firmware  FirmwareIterator
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zsuCrc32Start(ZSU::FirmwareIterator& firmware) {
    return libklug_mdu_ein_zsu_crc32_start(
      _lib, static_cast<firmware_iterator_handle>(firmware));
  }

  /**
   * Starts a ZSU crc32 result transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zsuCrc32Result() { return libklug_mdu_ein_zsu_crc32_result(_lib); }

  /**
   * Starts a ZSU crc32 result and exit transmission
   *
   * \details At some point after start, this will produce a \ref res::Status
   * result.
   *
   * \return true   Started
   * \return false  Busy
   */
  bool zsuCrc32ResultExit() {
    return libklug_mdu_ein_zsu_crc32_result_exit(_lib);
  }

private:
  // Internal CTor
  MDU_EIN(libklug_handle& lib) : _lib{lib} {}

  libklug_handle& _lib; ///< Underlying handle
};

/**
 * LibKLUG library wrapper
 *
 * \details Wraps libklug behind a class interface. As a limitation, since the
 * backend is dependent on a handle, an instance of this class can only be
 * moved.
 *
 */
struct LibKLUG {
  LibKLUG() : _lib{libklug_create()} {}
  LibKLUG(LibKLUG const&) = delete;
  LibKLUG& operator=(LibKLUG const&) = delete;

  LibKLUG(LibKLUG&& source) : _lib{source._lib} { source._lib = nullptr; }
  LibKLUG& operator=(LibKLUG&& source) = delete;
  ~LibKLUG() { libklug_destroy(_lib); }

  /** Register Cb (by class)
   *
   * \warning It is recommended to call \ref libKLUG::deregisterCb before
   * deconstructing the this object.
   *
   * \details Attemts to avoid a dangling pointer by storing a shared pointer
   * instead of a raw pointer.
   *
   * \todo Somehow, im unhappy with this implementation. Check if there is
   * another solution
   *
   * \tparam Unique  Type that guarantees uniqueness for each use
   * \tparam T       Type of class
   * \tparam F       Type of member function
   *
   * \param t shared pointer of `this`
   * \param f Method address (&T::method)
   */
  template<auto Unique = [] {}, typename T, typename F>
  constexpr auto registerCb(std::shared_ptr<T> t, F&& f) {
    static std::weak_ptr<T> _t_weak;
    _t_weak = t;

    static auto _f{f};

    _c_cb = [](::result r) {
      if (auto _t_shared = _t_weak.lock())
        return std::invoke(
          _f, _t_shared, std::forward<res::Result>(res::dispatch(r)));
      return;
    };

    libklug_register_cb(_lib, _c_cb);
  }

  /** Register Cb (by callable)
   *
   * \warning It is recommended to call \ref libKLUG::deregisterCb before the
   * callable becomes invalid for any reason
   *
   * \tparam Unique  Type that guarantees uniqueness for each use
   * \tparam F       Type of function object
   *
   * \param f Callable (e.g. Lambda)
   */
  template<auto Unique = [] {}, typename F>
  constexpr auto registerCb(F&& f) {
    static std::optional<F> _f;
    _f.emplace(std::forward<F>(f));
    _c_cb = [](::result r) {
      return std::invoke(*_f, std::forward<res::Result>(res::dispatch(r)));
    };
    libklug_register_cb(_lib, _c_cb);
  }

  /**
   * Deregisters a callback
   *
   * \note (actually just registers a nullptr, which is the same)
   *
   */
  void deregisterCb() { libklug_register_cb(_lib, nullptr); }

  /**
   * Get last result
   *
   * \warning Calling this before any transmission was started will result in a
   * deadlock.
   *
   * \return res::Result result
   */
  res::Result result() { return res::dispatch(libklug_result(_lib)); }

  /**
   * Init
   *
   * \return int
   * \retval Any error occurred
   */
  int init() { return libklug_init(_lib); }

  /**
   * Open device
   *
   * \param vid VID
   * \param pid PID
   *
   * \return int  Any error occurred
   */
  int open(uint16_t vid, uint16_t pid) { return libklug_open(_lib, vid, pid); }

  /**
   * Open device by File descripor
   *
   * \warning This exists only for the libusb backend.
   *
   * \param Fd  Filedescriptor
   *
   * \return int Any error occurred
   */
  int openFd(int Fd) { return libklug_openFd(_lib, Fd); }

  /**
   * Config device
   *
   * \return int  Any error occurred
   */
  int config() { return libklug_config(_lib); }

  /**
   * Claim device
   *
   * \return int  Any error occurred
   */
  int claim() { return libklug_claim(_lib); }

  /**
   * Release device
   *
   * \return int  Any error occurred
   */
  int release() { return libklug_release(_lib); }

  /**
   * Close device
   */
  void close() { return libklug_close(_lib); }

  /**
   * Get COM interface
   *
   * \return COM&
   */
  COM& com() { return _com; }

  /**
   * Get SUSIV2 interface
   *
   * \return SUSIV2&
   */
  SUSIV2& susiv2() { return _susiv2; }

  /**
   * Get MDU_EIN interface
   *
   * \return MDU_EIN&
   */
  MDU_EIN& mdu_ein() { return _mdu_ein; }

private:
  libklug_handle _lib; ///< Underlying handle

  bridge_callback _c_cb{nullptr}; ///< Bridge callback? is this needed?

  COM _com{_lib};         ///< COM interface
  SUSIV2 _susiv2{_lib};   ///< SUSIV2 interface
  MDU_EIN _mdu_ein{_lib}; ///< MDU_EIN interface
};

} // namespace libklug
