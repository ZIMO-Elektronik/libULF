#pragma once

#include "bridge_context.hpp"
#include "bridge_worker.hpp"

namespace bridge {

/**
 * Bridge for the MDU_EIN protocol
 *
 */
struct MDU_EIN {
  MDU_EIN(Context& ctx, Worker& worker);

  bool enterMDU();
  bool enterDCCZSU(uint32_t id = 0uz, uint32_t sn = 0uz, bool done = true);
  bool enterDCCZPP(uint32_t sn = 0uz, bool done = true);

  bool cvRead(uint16_t cv);
  bool cvWrite(uint16_t cv, uint8_t value);
  bool ping(uint32_t sn = 0, uint32_t id = 0);

private:
  Context& _ctx;    ///< Bridge context
  Worker& _worker;  ///< Worker
};

}  // namespace bridge