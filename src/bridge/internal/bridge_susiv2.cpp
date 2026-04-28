#include "bridge/internal/bridge_susiv2.hpp"

namespace bridge {

SUSIV2::SUSIV2(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

int SUSIV2::cvRead(uint16_t cv) { return -1; }

int SUSIV2::cvWrite(uint16_t cv, uint8_t value) { return -1; }

int SUSIV2::features() { return -1; }

}  // namespace bridge