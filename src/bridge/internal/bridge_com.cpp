#include "bridge/internal/bridge_com.hpp"
#include <algorithm>
#include "internal/transmission/com/base.hpp"

namespace bridge {

COM::COM(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

bool COM::ping() {
  return _worker.emplace<transmission::com::Base>(
    _ctx.connection, "PING\r", 2000u);
}

bool COM::reset() {
  return _worker.emplace<transmission::com::Base>(
    _ctx.connection, "RESET\r", 2000u);
}

bool COM::susiv2() {
  return _worker.emplace<transmission::com::Base>(
    _ctx.connection, "SUSIV2\r", 4000u);
}

}  // namespace bridge