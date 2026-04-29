#pragma once

#include "bridge_context.hpp"
#include "bridge_worker.hpp"

namespace bridge {

struct MDU_EIN {
  MDU_EIN(Context& ctx, Worker& worker);

  int enterMDU();
  int enterDCCZSU(uint32_t sn, bool done);
  int enterDCCZPP(uint32_t id = 0, uint32_t sn = 0, bool done = false);

  int cvRead(uint16_t cv);
  int cvWrite(uint16_t cv, uint8_t value);
  int ping(uint32_t id = 0, uint32_t sn = 0);

private:
  Context& _ctx;
  Worker& _worker;
};

}  // namespace bridge