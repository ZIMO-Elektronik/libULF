#include "bridge/internal/bridge_com.hpp"
#include <algorithm>
#include "internal/com_transmission.hpp"

namespace bridge {

COM::COM(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

bool COM::ping() {
  return _worker.emplace<transmission::COMTransmission>(
    _ctx.connection, "PING\r", 2000u);
}

bool COM::reset() {
  return _worker.emplace<transmission::COMTransmission>(
    _ctx.connection, "RESET\r", 2000u);
}

bool COM::susiv2() {
  return _worker.emplace<transmission::COMTransmission>(
    _ctx.connection, "SUSIV2\r", 4000u);
}

}  // namespace bridge