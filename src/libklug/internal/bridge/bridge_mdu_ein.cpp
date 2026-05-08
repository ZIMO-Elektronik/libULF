/**
 * Internal MDU_EIN bridge
 *
 * \file    src/libklug/internal/bridge/bridge_mdu_ein.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_mdu_ein.hpp"
#include <ulf/mdu_ein.hpp>
#include "libklug/internal/transmission/mdu_ein/base.hpp"
#include "libklug/internal/transmission/mdu_ein/cv_read.hpp"
#include "libklug/internal/transmission/mdu_ein/ping.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param ctx     Context
 * \param worker  Worker
 */
MDU_EIN::MDU_EIN(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

/**
 * MDU entry (async)
 *
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::enterMDU() {
  std::array<uint8_t, 16u> payload;
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::special2mdu_ein(ulf::mdu_ein::Command::Entry, 0u, payload),
    2000u);
}

/**
 * DCC ZSU entry (async)
 *
 * \param id    Decoder ID
 * \param sn    Decoder SN
 * \param done  true, if entry is done
 * \return true   Success
 * \return false  Busy
 */
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

/**
 * DCC ZPP entry (async)
 *
 * \param sn    Decoder SN
 * \param done  True, if entry is done
 * \return true   Success
 * \return false  Busy
 */
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

/**
 * Cv Read (async)
 *
 * \param cv  Cv address
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::cvRead(uint16_t cv) {
  return _worker.emplace<transmission::mdu_ein::CvRead>(_ctx.connection, cv);
}

/**
 * Cv Write (async)
 *
 * \param cv    Cv address
 * \param value Cv value
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::cvWrite(uint16_t cv, uint8_t value) {
  return _worker.emplace<transmission::mdu_ein::Base>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_cv_write_packet(cv, value)),
    2000u);
}

/**
 * Ping (async)
 *
 * \param sn  Decoder SN
 * \param id  Decoder ID
 * \return true   Success
 * \return false  Busy
 */
bool MDU_EIN::ping(uint32_t sn, uint32_t id) {
  return _worker.emplace<transmission::mdu_ein::Ping>(
    _ctx.connection,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_ping_packet(sn, id)),
    2000u);
}

} // namespace bridge
