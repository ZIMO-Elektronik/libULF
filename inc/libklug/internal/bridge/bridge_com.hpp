/**
 * Internal COM bridge
 *
 * \file    inc/libklug/internal/bridge/bridge_com.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <algorithm>
#include <expected>
#include "bridge_context.hpp"
#include "bridge_worker.hpp"

namespace bridge {

/**
 * Bridge for the ULF_COM protocol
 *
 */
struct COM {
  COM(Context& ctx, Worker& worker);

  std::expected<std::string, err::Error> ping();
  std::expected<bool, err::Error> reset();
  std::expected<bool, err::Error> susiv2();
  std::expected<bool, err::Error> mdu_ein();

private:
  Context& _ctx;   ///< Bridge context
  Worker& _worker; ///< Worker
};

} // namespace bridge
