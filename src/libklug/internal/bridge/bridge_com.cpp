/**
 * Internal COM bridge
 *
 * \file    src/libklug/internal/bridge/bridge_com.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_com.hpp"
#include <algorithm>
#include "config.hpp"
#include "libklug/internal/log/logger.hpp"
#include "libklug/internal/transmission/com/base.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param ctx     Context
 * \param worker  Worker
 */
COM::COM(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {
  LOG_INFO("COM bridge created");
}

/**
 * PING (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool COM::ping() {
  auto const r{_worker.emplace<transmission::com::Base>(
    _ctx.connection, "PING\r", internal::config::timeout::com::ping)};
  if (!r) LOG_WARN("COM ping transmission not emplaced, worker busy");
  return r;
}

/**
 * RESET (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool COM::reset() {
  auto const r{_worker.emplace<transmission::com::Base>(
    _ctx.connection, "RESET\r", internal::config::timeout::com::reset)};
  if (!r) LOG_WARN("COM reset transmission not emplaced, worker busy");
  return r;
}

/**
 * SUSIV2 (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool COM::susiv2() {
  auto const r{_worker.emplace<transmission::com::Base>(
    _ctx.connection, "SUSIV2\r", internal::config::timeout::com::susiv2)};
  if (!r) LOG_WARN("COM susiv2 transmission not emplaced, worker busy");
  return r;
}

/**
 * MDU_EIN (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool COM::mdu_ein() {
  auto const r{_worker.emplace<transmission::com::Base>(
    _ctx.connection, "MDU_EIN\r", internal::config::timeout::com::mdu_ein)};
  if (!r) LOG_WARN("COM mdu_ein transmission not emplaced, worker busy");
  return r;
}

} // namespace bridge
