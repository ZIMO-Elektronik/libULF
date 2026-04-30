#pragma once

#include <algorithm>
#include "bridge_context.hpp"
#include "bridge_worker.hpp"

namespace bridge {

/**
 * Bridge for the ULF_COM protocol
 *
 */
struct COM {
  COM(Context& ctx, Worker& worker);

  bool ping();
  bool reset();
  bool susiv2();
  bool mdu_ein();

private:
  Context& _ctx;    ///< Bridge context
  Worker& _worker;  ///< Worker
};

}  // namespace bridge