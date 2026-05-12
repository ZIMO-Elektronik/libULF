/**
 * LibKLUG C interface
 *
 * \file    src/libklug/libklug.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/libklug.h"
#include <cassert>
#include "libklug/callback/functor.hpp"
#include "libklug/internal/managed_iterator.hpp"
#include "libklug/libklug.hpp"
#include "libklug/result/dispatch.hpp"

bridge::Bridge* to_bridge(libklug_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle);
}

using FirmwareIterator = internal::ManagedIterator<std::vector<zsu::Firmware>>;

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

libklug_handle libklug_create(void) {
  return reinterpret_cast<libklug_instance*>(new bridge::Bridge());
}

void libklug_destroy(libklug_handle handle) { delete to_bridge(handle); }

void libklug_register_cb(libklug_handle handle, bridge_callback cb) {
  to_bridge(handle)->registerCB(std::make_unique<callback::Functor>(cb));
}

result libklug_result(libklug_handle handle) {
  return res::dispatch(to_bridge(handle)->result());
}

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

int libklug_com_ping(libklug_handle handle) {
  return to_bridge(handle)->com().ping();
}

int libklug_com_reset(libklug_handle handle) {
  return to_bridge(handle)->com().reset();
}

int libklug_com_susiv2(libklug_handle handle) {
  return to_bridge(handle)->com().susiv2();
}

int libklug_com_mdu_ein(libklug_handle handle) {
  return to_bridge(handle)->com().mdu_ein();
}

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

int libklug_susiv2_cv_read(libklug_handle handle, uint16_t cv) {
  return to_bridge(handle)->susiv2().cvRead(cv);
}

int libklug_susiv2_cv_write(libklug_handle handle, uint16_t cv, uint8_t value) {
  return to_bridge(handle)->susiv2().cvWrite(cv, value);
}

int libklug_susiv2_zpp_erase(libklug_handle handle) {
  return to_bridge(handle)->susiv2().zppErase();
}

int libklug_susiv2_zpp_write(libklug_handle handle,
                             zpp_handle file_handle,
                             uint32_t index) {
  return to_bridge(handle)->susiv2().zppWrite(
    reinterpret_cast<zpp::File*>(file_handle), index);
}

int libklug_susiv2_features(libklug_handle handle) {
  return to_bridge(handle)->susiv2().features();
}

int libklug_susiv2_exit(libklug_handle handle, int reboot, int cv8_reset) {
  return to_bridge(handle)->susiv2().exit(reboot == 0 ? false : true,
                                          cv8_reset == 0 ? false : true);
}

int libklug_susiv2_zpp_lc_dc_query(libklug_handle handle,
                                   zpp_handle file_handle) {
  return to_bridge(handle)->susiv2().zppLcDcQuery(
    reinterpret_cast<zpp::File*>(file_handle));
}

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

int libklug_mdu_ein_enter_mdu(libklug_handle handle) {
  return to_bridge(handle)->mdu_ein().enterMDU();
}

int libklug_mdu_ein_enter_dcc_zsu(libklug_handle handle) {
  return to_bridge(handle)->mdu_ein().enterDCCZSU();
}

int libklug_mdu_ein_enter_dcc_zpp(libklug_handle handle) {
  return to_bridge(handle)->mdu_ein().enterDCCZPP();
}

int libklug_mdu_ein_ping(libklug_handle handle) {
  return to_bridge(handle)->mdu_ein().ping();
}

int libklug_mdu_ein_config_transfer_rate(libklug_handle handle,
                                         uint8_t transfer_rate) {
  assert(transfer_rate >= 0u && transfer_rate <= 4u);
  return to_bridge(handle)->mdu_ein().configTransferRate(
    static_cast<mdu::TransferRate>(transfer_rate));
}

int libklug_mdu_ein_cv_read(libklug_handle handle, uint16_t cv) {
  return to_bridge(handle)->mdu_ein().cvRead(cv);
}

int libklug_mdu_ein_cv_write(libklug_handle handle,
                             uint16_t cv,
                             uint8_t value) {
  return to_bridge(handle)->mdu_ein().cvWrite(cv, value);
}

int libklug_mdu_ein_busy(libklug_handle handle) {
  return to_bridge(handle)->mdu_ein().busy();
}

int libklug_mdu_ein_zpp_valid_query(libklug_handle handle, zpp_handle zpp) {
  return to_bridge(handle)->mdu_ein().zppValidQuery(
    reinterpret_cast<zpp::File*>(zpp));
}

int libklug_mdu_ein_zpp_lc_dc_query(libklug_handle handle, zpp_handle zpp) {
  return to_bridge(handle)->mdu_ein().zppLcDcQuery(
    reinterpret_cast<zpp::File*>(zpp));
}

int libklug_mdu_ein_zpp_erase(libklug_handle handle, zpp_handle zpp) {
  return to_bridge(handle)->mdu_ein().zppErase(
    reinterpret_cast<zpp::File*>(zpp));
}

int libklug_mdu_ein_zpp_update(libklug_handle handle,
                               zpp_handle zpp,
                               uint32_t index) {
  return to_bridge(handle)->mdu_ein().zppUpdate(
    reinterpret_cast<zpp::File*>(zpp), index);
}

int libklug_mdu_ein_zpp_update_end(libklug_handle handle, zpp_handle zpp) {
  return to_bridge(handle)->mdu_ein().zppUpdateEnd(
    reinterpret_cast<zpp::File*>(zpp));
}

int libklug_mdu_ein_zpp_exit_reset(libklug_handle handle) {
  return to_bridge(handle)->mdu_ein().zppExitReset();
}

int libklug_mdu_ein_zsu_salsa20_iv(libklug_handle handle,
                                   firmware_iterator_handle firmware) {
  return to_bridge(handle)->mdu_ein().zsuSalsa20IV(
    reinterpret_cast<FirmwareIterator*>(firmware)->get());
}

int libklug_mdu_ein_zsu_erase(libklug_handle handle,
                              firmware_iterator_handle firmware) {
  return to_bridge(handle)->mdu_ein().zsuErase(
    reinterpret_cast<FirmwareIterator*>(firmware)->get());
}

int libklug_mdu_ein_zsu_update(libklug_handle handle,
                               firmware_iterator_handle firmware,
                               uint32_t index) {
  return to_bridge(handle)->mdu_ein().zsuUpdate(
    reinterpret_cast<FirmwareIterator*>(firmware)->get(), index);
}

int libklug_mdu_ein_zsu_crc32_start(libklug_handle handle,
                                    firmware_iterator_handle firmware) {
  return to_bridge(handle)->mdu_ein().zsuCRC32Start(
    reinterpret_cast<FirmwareIterator*>(firmware)->get());
}

int libklug_mdu_ein_zsu_crc32_result(libklug_handle handle) {
  return to_bridge(handle)->mdu_ein().zsuCRC32Result();
}

int libklug_mdu_ein_zsu_crc32_result_end(libklug_handle handle) {
  return to_bridge(handle)->mdu_ein().zsuCRC32ResultExit();
}

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
zpp_handle
libklug_zpp_read(libklug_handle handle, char16_t const* c, size_t length) {
  std::u16string_view s(c, length);

  return reinterpret_cast<zpp_handle>(
    reinterpret_cast<bridge::Bridge*>(handle)->zpp().read(
      std::filesystem::path{s}));
}

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
void libklug_zpp_release(libklug_handle handle, zpp_handle zpp) {
  return reinterpret_cast<bridge::Bridge*>(handle)->zpp().release(
    reinterpret_cast<zpp::File*>(zpp));
}

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
unsigned int libklug_zpp_blocks(libklug_handle handle, zpp_handle zpp) {
  return reinterpret_cast<bridge::Bridge*>(handle)->zpp().blocks(
    reinterpret_cast<zpp::File*>(zpp));
}

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

zsu_handle
libklug_zsu_read(libklug_handle handle, char const* c, size_t length) {
  std::string_view s{c, length};
  return reinterpret_cast<zsu_handle>(
    to_bridge(handle)->zsu().read(std::filesystem::path{s}));
}

void libklug_zsu_release(libklug_handle handle, zsu_handle zsu) {
  assert(handle);
  assert(zsu);
  return to_bridge(handle)->zsu().release(reinterpret_cast<zsu::File*>(zsu));
}

int libklug_zsu_firmware_next(firmware_iterator_handle firmware) {
  assert(firmware);
  return reinterpret_cast<FirmwareIterator*>(firmware)->next();
}

int libklug_zsu_firmware_previous(firmware_iterator_handle firmware) {
  assert(firmware);
  return reinterpret_cast<FirmwareIterator*>(firmware)->previous();
}

uint32_t libklug_zsu_firmware_id(firmware_iterator_handle firmware) {
  assert(firmware);
  return reinterpret_cast<FirmwareIterator*>(firmware)->get().id;
}

char const* libklug_zsu_firmware_name(firmware_iterator_handle firmware) {
  assert(firmware);
  return reinterpret_cast<FirmwareIterator*>(firmware)->get().name.data();
}

char const*
libklug_zsu_firmware_version_major(firmware_iterator_handle firmware) {
  assert(firmware);
  return reinterpret_cast<FirmwareIterator*>(firmware)
    ->get()
    .major_version.data();
}

char const*
libklug_zsu_firmware_version_minor(firmware_iterator_handle firmware) {
  assert(firmware);
  return reinterpret_cast<FirmwareIterator*>(firmware)
    ->get()
    .minor_version.data();
}

int libklug_zsu_firmware_type(firmware_iterator_handle firmware) {
  assert(firmware);
  return reinterpret_cast<FirmwareIterator*>(firmware)->get().type;
}

uint32_t libklug_zsu_firmware_blocks(libklug_handle handle,
                                     firmware_iterator_handle firmware) {
  assert(handle);
  assert(firmware);
  return to_bridge(handle)->zsu().blocks(
    reinterpret_cast<FirmwareIterator*>(firmware)->get());
}
