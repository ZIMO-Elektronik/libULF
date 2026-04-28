#include "bridge/internal/bridge_com.hpp"
#include <algorithm>
#include "internal/com_transmission.hpp"

namespace bridge {

COM::COM(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

int COM::ping() {
  return _worker.emplace<transmission::COMTransmission>(
           _ctx.connection, "PING\r", 2000u)
           ? 0
           : 1;
}

}  // namespace bridge