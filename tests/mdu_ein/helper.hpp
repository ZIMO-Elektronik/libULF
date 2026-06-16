#pragma once

#include <mdu/mdu.hpp>
#include <ulf/mdu_ein.hpp>

namespace helper::mdu {

namespace Subcommand {
enum Entry : uint8_t {
  MDU = 0u,
  DCC_ZSU = 1u,
  DCC_ZPP = 2u,
};

enum Speed : uint8_t {
  Fallback = 0u,
  Fast = 1u,
  Medium = 2u,
  Slow = 3u,
  Default = 4u,
};
} // namespace Subcommand

constexpr ulf::mdu_ein::Special make_mdu_entry_command() {
  return {.command = ulf::mdu_ein::Command::Entry,
          .subcommand = Subcommand::Entry::MDU,
          .payload{}};
}

constexpr ulf::mdu_ein::Special
make_dcc_zsu_entry_command(uint32_t id, uint32_t sn, bool more) {
  ulf::mdu_ein::Special s{.command = ulf::mdu_ein::Command::Entry,
                          .subcommand = Subcommand::Entry::DCC_ZSU,
                          .payload{}};
  s.payload.resize(sizeof(id) + sizeof(sn) + sizeof(bool));
  auto it{s.payload.begin()};
  it = ulf::mdu_ein::uint32_2data(id, it);
  it = ulf::mdu_ein::uint32_2data(sn, it);
  *it++ = more;
  return s;
}

constexpr ulf::mdu_ein::Special make_dcc_zpp_entry_command(uint32_t sn,
                                                           bool more) {
  ulf::mdu_ein::Special s{.command = ulf::mdu_ein::Command::Entry,
                          .subcommand = Subcommand::Entry::DCC_ZPP,
                          .payload{}};
  s.payload.resize(sizeof(sn) + sizeof(bool));
  auto it{s.payload.begin()};
  it = ulf::mdu_ein::uint32_2data(sn, it);
  *it++ = more;
  return s;
}

constexpr ulf::mdu_ein::Special make_speed_command(Subcommand::Speed speed) {
  return {
    .command = ulf::mdu_ein::Command::Speed, .subcommand = speed, .payload{}};
}

constexpr auto special2frame(ulf::mdu_ein::Special special) {
  return ulf::mdu_ein::special2mdu_ein(
    special.command,
    special.subcommand,
    std::span<uint8_t, 16u>{special.payload});
}

constexpr auto packet2frame(::mdu::Packet packet) {
  return ulf::mdu_ein::bytes2mdu_ein(packet);
}

constexpr auto receive_ack{
  [](uint8_t* buf, uint32_t len, int* rx_ed, uint8_t, uint32_t timeout) {
    auto const r{ulf::mdu_ein::response2mdu_ein(true, true)};
    assert(len >= r.size());
    std::ranges::copy(r, buf);
    *rx_ed = r.size();
    return 0;
  }};

constexpr auto receive_nak{
  [](uint8_t* buf, uint32_t len, int* rx_ed, uint8_t, uint32_t timeout) {
    auto const r{ulf::mdu_ein::response2mdu_ein(true, false)};
    assert(len >= r.size());
    std::ranges::copy(r, buf);
    *rx_ed = r.size();
    return 0;
  }};

} // namespace helper::mdu
