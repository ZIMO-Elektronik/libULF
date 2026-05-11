#pragma once

#include <cstdint>
#include <ranges>
#include <span>
#include <string_view>

namespace helper {

// ---
// Span conversion helper
// ---
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

// ---
// Flash data chunk helper
// ---
template<std::size_t... Is>
constexpr auto make_sequence(std::index_sequence<Is...>) {
  return std::array<uint8_t, sizeof...(Is)>{static_cast<uint8_t>(Is)...};
}

template<std::size_t S>
constexpr auto sequence{make_sequence(std::make_index_sequence<S>{})};
} // namespace helper
