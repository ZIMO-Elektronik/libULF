#pragma once

#include <algorithm>
#include "bridge_context.hpp"
#include "bridge_worker.hpp"

namespace bridge {

struct COM {
  COM(Context& ctx, Worker& worker);

  int ping(char* buffer, std::size_t length);

  int async_ping();

private:
  Context& _ctx;
  Worker& _worker;
};

}  // namespace bridge