#pragma once

#include "bridge_context.hpp"
#include "bridge_worker.hpp"

namespace bridge {

struct SUSIV2 {
  SUSIV2(Context& ctx, Worker& worker);

  bool cvRead(uint16_t cv);
  bool cvWrite(uint16_t cv, uint8_t value);
  bool features();

private:
  Context& _ctx;
  Worker& _worker;
};

}  // namespace bridge