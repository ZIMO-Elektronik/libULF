/**
 * Copyright (C) 2026 [ZIMO Elektronik]
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
 * Bridge context
 *
 * \file    src/bridge/bridge_context.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge_context.hpp"
#ifdef USE_LIBSERIALPORT
#  include "connection/libserialport_connection.hpp"
#elifdef USE_LIBUSB
#  include "connection/libusb_connection.hpp"
#endif

namespace bridge {

/**
 * CTor
 *
 */
Context::Context()

#ifdef USE_LIBSERIALPORT
  : connection{std::make_shared<internal::LibserialportConnection>()}
#elifdef USE_LIBUSB
  : connection{std::make_shared<internal::LibusbConnection>()}
#endif
{
}

/**
 * CTor
 *
 * \param conn Connection ptr
 */
Context::Context(std::shared_ptr<internal::IConnection> conn)
  : connection{conn} {}

} // namespace bridge
