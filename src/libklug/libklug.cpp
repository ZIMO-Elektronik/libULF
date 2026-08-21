/**
 * LibKLUG C interface
 *
 * \file    src/libklug/libklug.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/libklug.h"
#include <cassert>
#include <functional>
#include "libklug/callback/functor.hpp"
#include "libklug/internal/bridge/bridge.hpp"
#include "libklug/internal/bridge/bridge_zpp.hpp"
#include "libklug/internal/bridge/bridge_zsu.hpp"
#include "libklug/internal/exception/e_generic.hpp"
#include "libklug/result/dispatch.hpp"

bridge::Bridge* to_bridge(libklug_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle);
}

zpp::File* to_zpp(zpp_handle handle) {
  return reinterpret_cast<zpp::File*>(handle);
}

zsu::File* to_zsu(zsu_handle handle) {
  return reinterpret_cast<zsu::File*>(handle);
}

/// Template helper to dry code
template<typename F, typename R>
libklug_error execute(F&& operation, R* success) {
  auto const r{std::invoke(operation)};
  if (r) {
    *success = *r;
    return libklug_error::ok;
  }
  return libklug_error::unknown;
}

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

libklug_handle libklug_create(void) {
  return reinterpret_cast<libklug_instance*>(new bridge::Bridge());
}

void libklug_destroy(libklug_handle handle) { delete to_bridge(handle); }

int libklug_init(libklug_handle handle) { return to_bridge(handle)->init(); }

int libklug_open(libklug_handle handle, uint16_t vid, uint16_t pid) {
  return to_bridge(handle)->open(vid, pid);
}

int libklug_openFd(libklug_handle handle, int Fd) {
  return to_bridge(handle)->openFd(Fd);
}

int libklug_config(libklug_handle handle) {
  return to_bridge(handle)->config();
}

int libklug_claim(libklug_handle handle) { return to_bridge(handle)->claim(); }

int libklug_release(libklug_handle handle) {
  return to_bridge(handle)->release();
}

void libklug_close(libklug_handle handle) { return to_bridge(handle)->close(); }

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

libklug_error libklug_com_ping(libklug_handle hlib, char* buf, size_t len) {
  auto const r{to_bridge(hlib)->com().ping()};
  if (r) {
    std::copy_n(r->begin(), std::min(len, r->size()), buf);
    return libklug_error::ok;
  }
  return libklug_error::unknown;
}

libklug_error libklug_com_reset(libklug_handle hlib, int* success) {
  return execute([&]() { return to_bridge(hlib)->com().reset(); }, success);
}

libklug_error libklug_com_susiv2(libklug_handle hlib, int* success) {
  return execute([&]() { return to_bridge(hlib)->com().susiv2(); }, success);
}

libklug_error libklug_com_mdu_ein(libklug_handle hlib, int* success) {
  return execute([&]() { return to_bridge(hlib)->com().mdu_ein(); }, success);
}

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

libklug_error
libklug_susiv2_cv_read(libklug_handle hlib, uint16_t cv, uint8_t* value) {
  return execute([&]() { return to_bridge(hlib)->susiv2().cvRead(cv); }, value);
}

libklug_error libklug_susiv2_cv_write(libklug_handle hlib,
                                      uint16_t cv,
                                      uint8_t value,
                                      int* success) {
  return execute([&]() { return to_bridge(hlib)->susiv2().cvWrite(cv, value); },
                 success);
}

libklug_error libklug_susiv2_zpp_erase(libklug_handle hlib, int* success) {
  return execute([&]() { return to_bridge(hlib)->susiv2().zppErase(); },
                 success);
}

libklug_error libklug_susiv2_zpp_write(libklug_handle hlib,
                                       zpp_handle hzpp,
                                       uint32_t index,
                                       int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->susiv2().zppWrite(to_zpp(hzpp), index); },
    success);
}

libklug_error libklug_susiv2_features(libklug_handle hlib, int* success) {
  return execute([&]() { return to_bridge(hlib)->susiv2().features(); },
                 success);
}

libklug_error libklug_susiv2_exit(libklug_handle hlib,
                                  int reboot,
                                  int cv8_reset,
                                  int* success) {
  return execute(
    [&]() {
      return to_bridge(hlib)->susiv2().exit(reboot == 0 ? false : true,
                                            cv8_reset == 0 ? false : true);
    },
    success);
}

libklug_error libklug_susiv2_zpp_lc_dc_query(libklug_handle hlib,
                                             zpp_handle hzpp,
                                             int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->susiv2().zppLcDcQuery(to_zpp(hzpp)); },
    success);
}

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

libklug_error libklug_mdu_ein_enter_mdu(libklug_handle hlib, int* success) {
  return execute([&]() { return to_bridge(hlib)->mdu_ein().enterMDU(); },
                 success);
}

libklug_error libklug_mdu_ein_enter_dcc_zsu(
  libklug_handle hlib, uint32_t id, uint32_t sn, int done, int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->mdu_ein().enterDCCZSU(id, sn, done > 0); },
    success);
}

libklug_error libklug_mdu_ein_enter_dcc_zpp(libklug_handle hlib,
                                            uint32_t sn,
                                            int done,
                                            int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->mdu_ein().enterDCCZPP(sn, done > 0); },
    success);
}

libklug_error libklug_mdu_ein_ping(libklug_handle hlib,
                                   uint32_t sn,
                                   uint32_t id,
                                   int* success) {
  return execute([&]() { return to_bridge(hlib)->mdu_ein().ping(sn, id); },
                 success);
}

libklug_error libklug_mdu_ein_ping_all(libklug_handle hlib, int* success) {
  return execute([&]() { return to_bridge(hlib)->mdu_ein().ping(); }, success);
}

libklug_error libklug_mdu_ein_config_transfer_rate(libklug_handle hlib,
                                                   uint8_t transfer_rate,
                                                   int* success) {
  assert(transfer_rate >= 0u && transfer_rate <= 4u);
  return execute(
    [&]() {
      return to_bridge(hlib)->mdu_ein().configTransferRate(
        static_cast<mdu::TransferRate>(transfer_rate));
    },
    success);
}

libklug_error
libklug_mdu_ein_cv_read(libklug_handle hlib, uint16_t cv, uint8_t* success) {
  return execute([&]() { return to_bridge(hlib)->mdu_ein().cvRead(cv); },
                 success);
}

libklug_error libklug_mdu_ein_cv_write(libklug_handle hlib,
                                       uint16_t cv,
                                       uint8_t value,
                                       int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->mdu_ein().cvWrite(cv, value); }, success);
}

libklug_error libklug_mdu_ein_busy(libklug_handle hlib, int* success) {
  return execute([&]() { return to_bridge(hlib)->mdu_ein().busy(); }, success);
}

libklug_error libklug_mdu_ein_zpp_valid_query(libklug_handle hlib,
                                              zpp_handle hzpp,
                                              int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->mdu_ein().zppValidQuery(to_zpp(hzpp)); },
    success);
}

libklug_error libklug_mdu_ein_zpp_lc_dc_query(libklug_handle hlib,
                                              zpp_handle hzpp,
                                              int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->mdu_ein().zppLcDcQuery(to_zpp(hzpp)); },
    success);
}

libklug_error
libklug_mdu_ein_zpp_erase(libklug_handle hlib, zpp_handle hzpp, int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->mdu_ein().zppErase(to_zpp(hzpp)); },
    success);
}

libklug_error libklug_mdu_ein_zpp_update(libklug_handle hlib,
                                         zpp_handle hzpp,
                                         uint32_t index,
                                         int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->mdu_ein().zppUpdate(to_zpp(hzpp), index); },
    success);
}

libklug_error libklug_mdu_ein_zpp_update_end(libklug_handle hlib,
                                             zpp_handle hzpp,
                                             int* success) {
  return execute(
    [&]() { return to_bridge(hlib)->mdu_ein().zppUpdateEnd(to_zpp(hzpp)); },
    success);
}

libklug_error libklug_mdu_ein_zpp_exit_reset(libklug_handle hlib,
                                             int* success) {
  return execute([&]() { return to_bridge(hlib)->mdu_ein().zppExitReset(); },
                 success);
}

libklug_error libklug_mdu_ein_zsu_salsa20_iv(libklug_handle hlib,
                                             zsu_handle hzsu,
                                             size_t firmware_index,
                                             int* success) {
  return execute(
    [&]() {
      return to_bridge(hlib)->mdu_ein().zsuSalsa20IV(
        to_zsu(hzsu)->firmwares.at(firmware_index));
    },
    success);
}

libklug_error libklug_mdu_ein_zsu_erase(libklug_handle hlib,
                                        zsu_handle hzsu,
                                        size_t firmware_index,
                                        int* success) {
  return execute(
    [&]() {
      return to_bridge(hlib)->mdu_ein().zsuErase(
        to_zsu(hzsu)->firmwares.at(firmware_index));
    },
    success);
}

libklug_error libklug_mdu_ein_zsu_update(libklug_handle hlib,
                                         zsu_handle hzsu,
                                         size_t firmware_index,
                                         uint32_t index,
                                         int* success) {
  return execute(
    [&]() {
      return to_bridge(hlib)->mdu_ein().zsuUpdate(
        to_zsu(hzsu)->firmwares.at(firmware_index), index);
    },
    success);
}

libklug_error libklug_mdu_ein_zsu_crc32_start(libklug_handle hlib,
                                              zsu_handle hzsu,
                                              size_t firmware_index,
                                              int* success) {
  return execute(
    [&]() {
      return to_bridge(hlib)->mdu_ein().zsuCRC32Start(
        to_zsu(hzsu)->firmwares.at(firmware_index));
    },
    success);
}

libklug_error libklug_mdu_ein_zsu_crc32_result(libklug_handle hlib,
                                               int* success) {
  return execute([&]() { return to_bridge(hlib)->mdu_ein().zsuCRC32Result(); },
                 success);
}

libklug_error libklug_mdu_ein_zsu_crc32_result_exit(libklug_handle hlib,
                                                    int* success) {
  return execute([&]() { return to_bridge(hlib)->mdu_ein().zsuCRC32Result(); },
                 success);
}

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
zpp_handle libklug_zpp_read(char const* c, size_t length) {
  std::string_view s(c, length);
  return reinterpret_cast<zpp_handle>(
    bridge::ZPP::read(std::filesystem::path{s}));
}

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
void libklug_zpp_release(zpp_handle zpp) {
  return bridge::ZPP::release(reinterpret_cast<zpp::File*>(zpp));
}

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
unsigned int libklug_zpp_blocks(zpp_handle zpp) {
  return bridge::ZPP::blocks(reinterpret_cast<zpp::File*>(zpp));
}

char const* libklug_zpp_author(zpp_handle zpp) {
  return bridge::ZPP::author(reinterpret_cast<zpp::File*>(zpp)).data();
}

char const* libklug_zpp_email(zpp_handle zpp) {
  return bridge::ZPP::email(reinterpret_cast<zpp::File*>(zpp)).data();
}

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

zsu_handle libklug_zsu_read(char const* c, size_t length) {
  std::string_view s{c, length};
  return reinterpret_cast<zsu_handle>(
    bridge::ZSU::read(std::filesystem::path{s}));
}

void libklug_zsu_release(zsu_handle zsu) {
  assert(zsu);
  return bridge::ZSU::release(reinterpret_cast<zsu::File*>(zsu));
}

uint32_t libklug_zsu_get_firmware_count(zsu_handle const zsu) {
  assert(zsu);
  return reinterpret_cast<zsu::File*>(zsu)->firmwares.size();
}

uint32_t libklug_zsu_get_firmware_id(zsu_handle const zsu,
                                     size_t const firmware_index) {
  assert(zsu);
  return reinterpret_cast<zsu::File*>(zsu)->firmwares.at(firmware_index).id;
}

char const* libklug_zsu_get_firmware_name(zsu_handle const zsu,
                                          size_t const firmware_index) {
  assert(zsu);
  return reinterpret_cast<zsu::File*>(zsu)
    ->firmwares.at(firmware_index)
    .name.data();
}

char const*
libklug_zsu_get_firmware_major_version(zsu_handle const zsu,
                                       size_t const firmware_index) {
  assert(zsu);
  return reinterpret_cast<zsu::File*>(zsu)
    ->firmwares.at(firmware_index)
    .major_version.data();
}

char const*
libklug_zsu_get_firmware_minor_version(zsu_handle const zsu,
                                       size_t const firmware_index) {
  assert(zsu);
  return reinterpret_cast<zsu::File*>(zsu)
    ->firmwares.at(firmware_index)
    .minor_version.data();
}

int libklug_zsu_get_firmware_type(zsu_handle const zsu,
                                  size_t const firmware_index) {
  assert(zsu);
  return reinterpret_cast<zsu::File*>(zsu)->firmwares.at(firmware_index).type;
}

uint32_t libklug_zsu_get_firmware_block_count(zsu_handle const zsu,
                                              size_t const firmware_index) {
  assert(zsu);
  return bridge::ZSU::blocks(
    reinterpret_cast<zsu::File*>(zsu)->firmwares.at(firmware_index));
}
uint8_t const* libklug_zsu_get_firmware_data(zsu_handle const zsu,
                                             size_t const firmware_index) {
  assert(zsu);
  return reinterpret_cast<zsu::File*>(zsu)
    ->firmwares.at(firmware_index)
    .bin.data();
}

size_t libklug_zsu_get_firmware_data_size(zsu_handle const zsu,
                                          size_t const firmware_index) {
  assert(zsu);
  return reinterpret_cast<zsu::File*>(zsu)
    ->firmwares.at(firmware_index)
    .bin.size();
}
