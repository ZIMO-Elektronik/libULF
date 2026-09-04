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
 * Internal bridge context
 *
 * \file    src/bridge/bridge_context.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <future>
#include <mutex>
#include "connection/i_connection.hpp"
#include "transmission/transmission_base.hpp"

namespace bridge {

/**
 * Bridge context
 *
 * \details Holds the current transmission state, as well as the Connection info
 *
 * \todo Maybe construct a state of connection to use later on.
 *
 */
struct Context {
  Context();
  Context(std::shared_ptr<internal::IConnection> conn);

  std::shared_ptr<internal::IConnection> connection; ///< Libusb connection info
};

} // namespace bridge
