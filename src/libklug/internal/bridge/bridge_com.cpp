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
 */
COM::COM(Context& ctx) : _ctx{ctx} { LOG_INFO("COM bridge created"); }

/**
 * PING
 *
 * \note
 * Will throw an exception on error
 *
 * \retval std::string  Response
 */
std::string COM::ping() {
  transmission::com::Base t{
    _ctx.connection, "PING\r", internal::config::timeout::com::ping};
  t.execute();
  return t.evaluateString();
}

/**
 * RESET
 *
 * \note
 * Will throw an exception on error
 *
 * \return bool Response
 */
bool COM::reset() {
  transmission::com::Base t{
    _ctx.connection, "RESET\r", internal::config::timeout::com::reset};
  t.execute();
  return t.evaluateBool();
}

/**
 * SUSIV2
 *
 * \note
 * Will throw an exception on error
 *
 * \return bool Response
 */
bool COM::susiv2() {
  transmission::com::Base t{
    _ctx.connection, "SUSIV2\r", internal::config::timeout::com::susiv2};
  t.execute();
  return t.evaluateBool();
}

/**
 * MDU_EIN
 *
 * \note
 * Will throw an exception on error
 *
 * \return bool Response
 */
bool COM::mdu_ein() {
  transmission::com::Base t{
    _ctx.connection, "MDU_EIN\r", internal::config::timeout::com::mdu_ein};
  t.execute();
  return t.evaluateBool();
}

} // namespace bridge
