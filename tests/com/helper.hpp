#pragma once

#include <algorithm>
#include <cstdint>
#include "../helper.hpp"

namespace helper::com {

constexpr auto receive_ok{
  [](uint8_t* buf, uint32_t len, int* rx_ed, uint8_t, uint32_t timeout) {
    auto const r{helper::string_view2span("OK\r")};
    assert(len >= r.size());
    std::ranges::copy(r, buf);
    *rx_ed = r.size();
    return 0;
  }};

constexpr auto receive_not_ok{
  [](uint8_t* buf, uint32_t len, int* rx_ed, uint8_t, uint32_t timeout) {
    auto const r{helper::string_view2span("NOT_OK\r")};
    assert(len >= r.size());
    std::ranges::copy(r, buf);
    *rx_ed = r.size();
    return 0;
  }};

} // namespace helper::com
