#include "bridge/internal/bridge_mdu_ein.hpp"
#include <ulf/mdu_ein.hpp>
#include "internal/transmission/mdu_ein/base.hpp"
#include "internal/transmission/mdu_ein/cv_read.hpp"
#include "internal/transmission/mdu_ein/ping.hpp"

namespace bridge {

MDU_EIN::MDU_EIN(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

bool MDU_EIN::enterMDU() {
  std::array<uint8_t, 16u> payload;
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(ulf::mdu_ein::Command::Entry, 0u, payload),
    2000u);
}

bool MDU_EIN::enterDCCZSU(uint32_t id, uint32_t sn, bool done) {
  std::vector<uint8_t> payload{};
  auto it{std::back_inserter(payload)};
  ulf::mdu_ein::uint32_2data(id, it);
  ulf::mdu_ein::uint32_2data(sn, it);
  *it = done ? 0u : 1u;
  payload.resize(16);
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(
      ulf::mdu_ein::Command::Entry, 1u, std::span<uint8_t, 16u>{payload}),
    2000u);
}

bool MDU_EIN::enterDCCZPP(uint32_t sn, bool done) {
  std::vector<uint8_t> payload{};
  auto it{std::back_inserter(payload)};
  ulf::mdu_ein::uint32_2data(sn, it);
  *it = done ? 0u : 1u;
  payload.resize(16);
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(
      ulf::mdu_ein::Command::Entry, 2u, std::span<uint8_t, 16u>{payload}),
    2000u);
}

bool MDU_EIN::cvRead(uint16_t cv) {
  return _worker.emplace<transmission::mdu_ein::CvRead>(_ctx.connection, cv);
}

bool MDU_EIN::cvWrite(uint16_t cv, uint8_t value) { return false; }

bool MDU_EIN::ping(uint32_t sn, uint32_t id) {
  return _worker.emplace<transmission::mdu_ein::Ping>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_ping_packet(sn, id)),
    2000u);
}

}  // namespace bridge