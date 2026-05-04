/**
 * Bridge interface
 *
 * \file    src/bridge/bridge.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge/bridge.hpp"
#include "bridge/internal/bridge.hpp"

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

bridge_handle bridge_create(void) {
  return reinterpret_cast<bridge_instance*>(new bridge::Bridge());
}

void bridge_destroy(bridge_handle handle) {
  delete reinterpret_cast<bridge::Bridge*>(handle);
}

void bridge_register_cb(bridge_handle handle, bridge_callback cb) {
  reinterpret_cast<bridge::Bridge*>(handle)->registerCB(cb);
}

result_t bridge_result(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->result();
}

int bridge_init(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->init();
}

int bridge_open(bridge_handle handle, uint16_t vid, uint16_t pid) {
  return reinterpret_cast<bridge::Bridge*>(handle)->open(vid, pid);
}

int bridge_openFd(bridge_handle handle, int Fd) {
  return reinterpret_cast<bridge::Bridge*>(handle)->openFd(Fd);
}

int bridge_config(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->config();
}

int bridge_claim(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->claim();
}

int bridge_release(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->release();
}

void bridge_close(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->close();
}

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

int bridge_com_ping(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->com().ping();
}

int bridge_com_reset(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->com().reset();
}

int bridge_com_susiv2(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->com().susiv2();
}

int bridge_com_mdu_ein(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->com().mdu_ein();
}

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

int bridge_susiv2_cv_read(bridge_handle handle, uint16_t cv) {
  return reinterpret_cast<bridge::Bridge*>(handle)->susiv2().cvRead(cv);
}

int bridge_susiv2_features(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->susiv2().features();
}

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

int bridge_mdu_ein_enter_mdu(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->mdu_ein().enterMDU();
}

int bridge_mdu_ein_enter_dcc_zsu(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->mdu_ein().enterDCCZSU();
}

int bridge_mdu_ein_enter_dcc_zpp(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->mdu_ein().enterDCCZPP();
}

int bridge_mdu_ein_cv_read(bridge_handle handle, uint16_t cv) {
  return reinterpret_cast<bridge::Bridge*>(handle)->mdu_ein().cvRead(cv);
}

int bridge_mdu_ein_ping(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->mdu_ein().ping();
}

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
zpp_handle
bridge_zpp_read(bridge_handle b_handle, char16_t const* c, size_t length) {
  std::u16string_view s(c, length);

  return reinterpret_cast<zpp_handle>(
    reinterpret_cast<bridge::Bridge*>(b_handle)->zpp().read(
      std::filesystem::path{s}));
}

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
void bridge_zpp_release(bridge_handle b_handle, zpp_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(b_handle)->zpp().release(
    reinterpret_cast<zpp::File*>(handle));
}

/// \todo maybe, it is unnecessary to let this run over bridge since ZPP could
/// be a static class
unsigned int bridge_zpp_blocks(bridge_handle b_handle, zpp_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(b_handle)->zpp().blocks(
    reinterpret_cast<zpp::File*>(handle));
}