/**
 * LibKLUG C interface
 *
 * \file    src/libklug/libklug.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/libklug.h"
#include "libklug/callback/functor.hpp"
#include "libklug/libklug.hpp"
#include "libklug/result/dispatch.hpp"

bridge::Bridge* to_bridge(libklug_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle);
}

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

int libklug_mdu_ein_cv_read(libklug_handle handle, uint16_t cv) {
  return to_bridge(handle)->mdu_ein().cvRead(cv);
}

int libklug_mdu_ein_ping(libklug_handle handle) {
  return to_bridge(handle)->mdu_ein().ping();
}

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
zpp_handle
libklug_zpp_read(libklug_handle b_handle, char16_t const* c, size_t length) {
  std::u16string_view s(c, length);

  return reinterpret_cast<zpp_handle>(
    reinterpret_cast<bridge::Bridge*>(b_handle)->zpp().read(
      std::filesystem::path{s}));
}

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
void libklug_zpp_release(libklug_handle b_handle, zpp_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(b_handle)->zpp().release(
    reinterpret_cast<zpp::File*>(handle));
}

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
unsigned int libklug_zpp_blocks(libklug_handle b_handle, zpp_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(b_handle)->zpp().blocks(
    reinterpret_cast<zpp::File*>(handle));
}
