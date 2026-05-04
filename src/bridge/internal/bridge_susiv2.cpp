/**
 * Internal SUSIV2 bridge
 *
 * \file    src/bridge/internal/bridge_susiv2.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge/internal/bridge_susiv2.hpp"
#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>
#include "internal/transmission/susiv2/base.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param ctx     Context
 * \param worker  Worker
 */
SUSIV2::SUSIV2(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

/**
 * Cv Read (async)
 *
 * \param cv  Cv address
 * \return true   Success
 * \return false  Busy
 */
bool SUSIV2::cvRead(uint16_t cv) {
  return _worker.emplace<transmission::susiv2::Base>(
    _ctx.connection,
    ulf::susiv2::packet2frame<std::vector<uint8_t>>(
      zusi::make_cv_read_packet(0, cv)),
    2000uz);
}

/**
 * Cv Write (async)
 *
 * \param cv    Cv address
 * \param value Cv value
 * \return true   Success
 * \return false  Busy
 *
 * \todo Implement
 */
bool SUSIV2::cvWrite(uint16_t cv, uint8_t value) {
  assert(false);
  return -1;
}

/**
 * Feature request (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool SUSIV2::features() {
  return _worker.emplace<transmission::susiv2::Base>(
    _ctx.connection,
    ulf::susiv2::packet2frame<std::vector<uint8_t>>(
      zusi::make_features_packet()),
    2000uz);
}

}  // namespace bridge
