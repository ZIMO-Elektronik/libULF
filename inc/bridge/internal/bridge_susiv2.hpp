/**
 * Internal SUSIV2 bridge
 *
 * \file    inc/bridge/internal/bridge_susiv2.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "bridge_context.hpp"
#include "bridge_worker.hpp"

namespace bridge {

/**
 * Bridge for the SUSIV2 protocol
 *
 */
struct SUSIV2 {
  SUSIV2(Context& ctx, Worker& worker);

  bool cvRead(uint16_t cv);
  bool cvWrite(uint16_t cv, uint8_t value);
  bool features();

private:
  Context& _ctx;    ///< Bridge context
  Worker& _worker;  ///< Worker
};

}  // namespace bridge