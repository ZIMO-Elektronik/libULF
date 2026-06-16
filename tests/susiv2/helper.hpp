#pragma once

#include <algorithm>
#include <cstdint>
#include <ulf/susiv2.hpp>
#include <vector>

namespace helper::susiv2 {

constexpr auto receive_ack{
  [](uint8_t* buf, uint32_t len, int* rx_ed, uint32_t timeout) {
    std::vector<uint8_t> r{ulf::susiv2::ack};
    r.push_back(zusi::crc8(r));
    assert(len >= r.size());
    std::ranges::copy(r, buf);
    *rx_ed = r.size();
    return 0;
  }};

constexpr auto receive_nak{
  [](uint8_t* buf, uint32_t len, int* rx_ed, uint32_t timeout) {
    std::vector<uint8_t> r{ulf::susiv2::nak};
    r.push_back(zusi::crc8(r));
    assert(len >= r.size());
    std::ranges::copy(r, buf);
    *rx_ed = r.size();
    return 0;
  }};

} // namespace helper::susiv2
