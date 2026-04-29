#pragma once

#include <algorithm>
#include "bridge_context.hpp"
#include "bridge_worker.hpp"

namespace bridge {

struct COM {
  COM(Context& ctx, Worker& worker);

  bool ping();
  bool reset();
  bool susiv2();

private:
  Context& _ctx;
  Worker& _worker;
};

}  // namespace bridge