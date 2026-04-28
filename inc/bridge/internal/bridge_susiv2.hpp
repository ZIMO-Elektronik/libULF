#pragma once

#include "bridge_context.hpp"
#include "bridge_worker.hpp"

namespace bridge {

struct SUSIV2 {
  SUSIV2(Context& ctx, Worker& worker);

  int cvRead(uint16_t cv);
  int cvWrite(uint16_t cv, uint8_t value);
  int features();

private:
  Context& _ctx;
  Worker& _worker;
};

}  // namespace bridge