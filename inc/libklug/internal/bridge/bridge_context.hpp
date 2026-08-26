/**
 * Internal bridge context
 *
 * \file    inc/libklug/internal/bridge/bridge_context.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <future>
#include <mutex>
#include "libklug/internal/connection/i_connection.hpp"
#include "libklug/internal/transmission/transmission_base.hpp"

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
