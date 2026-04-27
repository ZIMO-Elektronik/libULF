#pragma once

#include <algorithm>
#include "bridge_context.hpp"

namespace bridge {

struct COM {
  COM(Context& ctx);

  int ping(char* buffer, std::size_t length);

private:
  Context& _ctx;
};

}  // namespace bridge