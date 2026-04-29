#include "bridge/internal/bridge_susiv2.hpp"
#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>
#include "internal/susiv2_transmission.hpp"

namespace bridge {

SUSIV2::SUSIV2(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

bool SUSIV2::cvRead(uint16_t cv) {
  return _worker.emplace<transmission::SUSIV2Transmission>(
    _ctx.connection,
    ulf::susiv2::packet2frame<std::vector<uint8_t>>(
      zusi::make_cv_read_packet(0, cv)),
    2000uz);
}

bool SUSIV2::cvWrite(uint16_t cv, uint8_t value) {
  assert(false);
  return -1;
}

bool SUSIV2::features() {
  return _worker.emplace<transmission::SUSIV2Transmission>(
    _ctx.connection,
    ulf::susiv2::packet2frame<std::vector<uint8_t>>(
      zusi::make_features_packet()),
    2000uz);
}

}  // namespace bridge
