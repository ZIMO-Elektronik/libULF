/**
 * Copyright (C) 2026 ZIMO Elektronik
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 *
 *
 *
 *
 *
 * Internal COM bridge
 *
 * \file    src/bridge/bridge_com.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge_com.hpp"
#include <algorithm>
#include "config.hpp"
#include "log/logger.hpp"
#include "transmission/com/base.hpp"

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
 * \retval std::string  Response
 *
 * \throws ulf_error   First error occurred
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
 * \return bool Response
 *
 * \throws ulf_error   First error occurred
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
 * \return bool Response
 *
 * \throws ulf_error   First error occurred
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
 * \return bool Response
 *
 * \throws ulf_error   First error occurred
 */
bool COM::mdu_ein() {
  transmission::com::Base t{
    _ctx.connection, "MDU_EIN\r", internal::config::timeout::com::mdu_ein};
  t.execute();
  return t.evaluateBool();
}

} // namespace bridge
