#pragma once

#include <cstdint>
#include <ranges>
#include <span>
#include <string_view>

namespace helper {

constexpr std::span<uint8_t const> string_view2span(std::string_view const& s) {
  return {std::bit_cast<uint8_t*>(s.data()), s.size()};
}

template<std::ranges::input_range T>
requires requires(T t) {
  requires std::constructible_from<std::span<uint8_t const>,
                                   decltype(t.data()),
                                   decltype(t.size())>;
}
constexpr std::span<uint8_t const> range2span(T&& t) {
  return {t.data(), t.size()};
}
} // namespace helper
