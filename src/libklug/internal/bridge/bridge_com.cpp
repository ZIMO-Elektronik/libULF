/**
 * Internal COM bridge
 *
 * \file    src/libklug/internal/bridge/bridge_com.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_com.hpp"
#include <algorithm>
#include "libklug/internal/transmission/com/base.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param ctx     Context
 * \param worker  Worker
 */
COM::COM(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

/**
 * PING (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool COM::ping() {
  return _worker.emplace<transmission::com::Base>(
    _ctx.connection, "PING\r", 2000u);
}

/**
 * RESET (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool COM::reset() {
  return _worker.emplace<transmission::com::Base>(
    _ctx.connection, "RESET\r", 2000u);
}

/**
 * SUSIV2 (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool COM::susiv2() {
  return _worker.emplace<transmission::com::Base>(
    _ctx.connection, "SUSIV2\r", 4000u);
}

/**
 * MDU_EIN (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool COM::mdu_ein() {
  return _worker.emplace<transmission::com::Base>(
    _ctx.connection, "MDU_EIN\r", 4000u);
}

} // namespace bridge
