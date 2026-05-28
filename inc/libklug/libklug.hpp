/**
 * Libklug C++ wrapper
 *
 * \file    inc/libklug/libklug.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
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

enum class Speed : uint8_t {
  Fallback = 0u,
  Fast = 1u,
  Medium = 2u,
  Slow = 3u,
  Default = 4u,
};

} // namespace mdu

struct ZPP {
  ZPP(std::filesystem::path path)
    : _zpp{libklug_zpp_read(path.string().data(), path.string().size())} {}
  ZPP(ZPP const&) = delete;
  ~ZPP() {
    if (_zpp) libklug_zpp_release(_zpp);
  }

  unsigned int blocks() { return libklug_zpp_blocks(_zpp); }

  explicit operator zpp_handle() { return _zpp; }

private:
  zpp_handle _zpp;
};

struct ZSU {
  struct FirmwareIterator {
    FirmwareIterator(zsu_handle zsu)
      : _fwIt{libklug_zsu_firmware_iterator_create_begin(zsu)} {}
    FirmwareIterator(zsu_handle zsu, bool)
      : _fwIt{libklug_zsu_firmware_iterator_create_end(zsu)} {}
    FirmwareIterator(FirmwareIterator const&) = delete;
    ~FirmwareIterator() { libklug_zsu_destroy_firmware_iterator(_fwIt); }

    explicit operator firmware_iterator_handle() { return _fwIt; }
    explicit operator firmware_iterator_handle const() const { return _fwIt; }

    FirmwareIterator& operator++() {
      libklug_zsu_firmware_iterator_next(_fwIt);
      return *this;
    }

    FirmwareIterator& operator--() {
      libklug_zsu_firmware_iterator_previous(_fwIt);
      return *this;
    }

    bool operator==(FirmwareIterator const& rhs) const {
      return libklug_zsu_firmware_iterator_equals(
        static_cast<firmware_iterator_handle const>(*this),
        static_cast<firmware_iterator_handle const>(rhs));
    }

    FirmwareIterator operator++(int) = delete;
    FirmwareIterator operator--(int) = delete;

    uint32_t id() { return libklug_zsu_firmware_iterator_get_id(_fwIt); }
    std::string_view name() {
      return {libklug_zsu_firmware_iterator_get_name(_fwIt)};
    }
    std::string_view versionMajor() {
      return {libklug_zsu_firmware_iterator_get_version_major(_fwIt)};
    }
    std::string_view versionMinor() {
      return {libklug_zsu_firmware_iterator_get_version_minor(_fwIt)};
    }
    int type() { return libklug_zsu_firmware_iterator_get_type(_fwIt); }
    unsigned int blocks() {
      return libklug_zsu_firmware_iterator_get_blocks(_fwIt);
    }

  private:
    firmware_iterator_handle _fwIt;
  };

  using iterator = FirmwareIterator;

  ZSU(std::filesystem::path path)
    : _zsu{libklug_zsu_read(path.string().data(), path.string().size())} {}
  ZSU(ZSU const&) = delete;
  ~ZSU() {
    if (_zsu) libklug_zsu_release(_zsu);
  }

  explicit operator zsu_handle() { return _zsu; }

  iterator begin() { return FirmwareIterator{_zsu}; }
  iterator end() { return FirmwareIterator{_zsu, bool{}}; }

private:
  zsu_handle _zsu;
};

struct COM {
  COM(libklug_handle& lib) : _lib{lib} {}
  ~COM() = default;

  int ping() { return libklug_com_ping(_lib); }
  int reset() { return libklug_com_reset(_lib); }
  int susiv2() { return libklug_com_susiv2(_lib); }
  int mdu_ein() { return libklug_com_mdu_ein(_lib); }

private:
  libklug_handle& _lib;
};

struct SUSIV2 {
  SUSIV2(libklug_handle& lib) : _lib{lib} {}
  ~SUSIV2() = default;

  int cvRead(uint16_t cv) { return libklug_susiv2_cv_read(_lib, cv); }
  int cvWrite(uint16_t cv, uint8_t value) {
    return libklug_susiv2_cv_write(_lib, cv, value);
  }
  int zppErase() { return libklug_susiv2_zpp_erase(_lib); }
  int zppWrite(ZPP& zpp, uint32_t index) {
    return libklug_susiv2_zpp_write(_lib, static_cast<zpp_handle>(zpp), index);
  }
  int zppLcDcQuery(ZPP& zpp) {
    return libklug_susiv2_zpp_lc_dc_query(_lib, static_cast<zpp_handle>(zpp));
  }

private:
  libklug_handle& _lib;
};

struct MDU_EIN {
  MDU_EIN(libklug_handle& lib) : _lib{lib} {}
  ~MDU_EIN() = default;

  int enterMDU() { return libklug_mdu_ein_enter_mdu(_lib); }
  int enterDCCZSU() { return libklug_mdu_ein_enter_dcc_zsu(_lib); }
  int enterDCCZPP() { return libklug_mdu_ein_enter_dcc_zpp(_lib); }

  int ping(uint32_t sn, uint32_t id) {
    return libklug_mdu_ein_ping(_lib, sn, id);
  }
  int configTransferRate(mdu::Speed speed) {
    return libklug_mdu_ein_config_transfer_rate(_lib,
                                                std::to_underlying(speed));
  }
  int cvRead(uint16_t cv) { return libklug_mdu_ein_cv_read(_lib, cv); }
  int cvWrite(uint16_t cv, uint8_t value) {
    return libklug_mdu_ein_cv_write(_lib, cv, value);
  }
  int busy() { return libklug_mdu_ein_busy(_lib); }

  int zppValidQuery(ZPP& zpp) {
    return libklug_mdu_ein_zpp_valid_query(_lib, static_cast<zpp_handle>(zpp));
  }
  int zppLcDcQuery(ZPP& zpp) {
    return libklug_mdu_ein_zpp_lc_dc_query(_lib, static_cast<zpp_handle>(zpp));
  }
  int zppErase(ZPP& zpp) {
    return libklug_mdu_ein_zpp_erase(_lib, static_cast<zpp_handle>(zpp));
  }
  int zppUpdate(ZPP& zpp, uint32_t index) {
    return libklug_mdu_ein_zpp_update(
      _lib, static_cast<zpp_handle>(zpp), index);
  }
  int zppUpdateEnd(ZPP& zpp) {
    return libklug_mdu_ein_zpp_update_end(_lib, static_cast<zpp_handle>(zpp));
  }
  int zppExitReset() { return libklug_mdu_ein_zpp_exit_reset(_lib); }

  int zsuSalsa20Iv(ZSU::FirmwareIterator& firmware) {
    return libklug_mdu_ein_zsu_salsa20_iv(
      _lib, static_cast<firmware_iterator_handle>(firmware));
  }
  int zsuErase(ZSU::FirmwareIterator& firmware) {
    return libklug_mdu_ein_zsu_erase(
      _lib, static_cast<firmware_iterator_handle>(firmware));
  }
  int zsuUpdate(ZSU::FirmwareIterator& firmware, uint32_t index) {
    return libklug_mdu_ein_zsu_update(
      _lib, static_cast<firmware_iterator_handle>(firmware), index);
  }
  int zsuCrc32Start(ZSU::FirmwareIterator& firmware) {
    return libklug_mdu_ein_zsu_crc32_start(
      _lib, static_cast<firmware_iterator_handle>(firmware));
  }
  int zsuCrc32Result() { return libklug_mdu_ein_zsu_crc32_result(_lib); }
  int zsuCrc32ResultExit() {
    return libklug_mdu_ein_zsu_crc32_result_exit(_lib);
  }

private:
  libklug_handle& _lib;
};

struct LibKLUG {
  LibKLUG() : _lib{libklug_create()} {}
  LibKLUG(LibKLUG const&) = delete;
  ~LibKLUG() { libklug_destroy(_lib); }

  /// Register Cb
  ///
  /// \tparam Unique  Type that guarantees uniqueness for each use
  /// \tparam T       Type of class
  /// \tparam F       Type of member function
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

  /// Register Cb
  ///
  /// \tparam Unique  Type that guarantees uniqueness for each use
  /// \tparam F       Type of function object
  template<auto Unique = [] {}, typename F>
  constexpr auto registerCb(F&& f) {
    static std::optional<F> _f;
    _f.emplace(std::forward<F>(f));
    _c_cb = [](::result r) {
      return std::invoke(*_f, std::forward<res::Result>(res::dispatch(r)));
    };
    libklug_register_cb(_lib, _c_cb);
  }

  void deregisterCb() { libklug_register_cb(_lib, nullptr); }

  res::Result result() { return res::dispatch(libklug_result(_lib)); }

  int init() { return libklug_init(_lib); }
  int open(uint16_t vid, uint16_t pid) { return libklug_open(_lib, vid, pid); }
  int openFd(int Fd) { return libklug_openFd(_lib, Fd); }
  int config() { return libklug_config(_lib); }
  int claim() { return libklug_claim(_lib); }
  int release() { return libklug_release(_lib); }
  void close() { return libklug_close(_lib); }

  COM& com() { return _com; }
  SUSIV2& susiv2() { return _susiv2; }
  MDU_EIN& mdu_ein() { return _mdu_ein; }

private:
  libklug_handle _lib;

  bridge_callback _c_cb{nullptr};

  COM _com{_lib};
  SUSIV2 _susiv2{_lib};
  MDU_EIN _mdu_ein{_lib};
};

} // namespace libklug
