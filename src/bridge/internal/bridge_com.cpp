#include "bridge/internal/bridge_com.hpp"
#include <algorithm>
#include "internal/com_transmission.hpp"

namespace bridge {

COM::COM(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

int COM::ping(char* buffer, size_t length) {
  transmission::COMTransmission transmission{_ctx.connection, "PING\r", 2000u};
  auto rc{transmission.execute()};
  if (rc != 0) return rc;

  auto const result{transmission.result()};
  if (result.size() > length) return 1;  // We dont have enough space in buffer

  std::ranges::copy(result, buffer);
  return 0;
}

int COM::async_ping() {
  return _worker.emplace<transmission::COMTransmission>(
           _ctx.connection, "PING\r", 2000u)
           ? 0
           : 1;
}

}  // namespace bridge