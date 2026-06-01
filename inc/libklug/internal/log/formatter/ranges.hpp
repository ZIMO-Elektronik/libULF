#pragma once

#include <algorithm>
#include <concepts>
#include <format>
#include <ranges>

template<typename T>
concept SpanConvertible = std::convertible_to<T, std::span<uint8_t const>>;

template<typename T>
requires SpanConvertible<T>
struct std::formatter<T, char> {
  bool hex{false};

  constexpr auto parse(std::format_parse_context& ctx) {
    auto it = ctx.begin();
    if (it != ctx.end() && (*it == 'x' || *it == 'X')) {
      hex = true;
      it++;
    }
    if (it != ctx.end() && *it != '}') {
      throw std::format_error("Ungültige Format-Option für Byte-Container.");
    }
    return it;
  }

  auto format(T const& container, std::format_context& ctx) const {
    // Explizite/Implizite Konvertierung in das gewünschte Span
    std::span<uint8_t const> sp = container;

    auto out = ctx.out();
    out = std::format_to(out, "[");

    for (size_t i = 0; i < sp.size(); ++i) {
      if (hex) {
        out = std::format_to(out, "{:02X}", sp[i]);
      } else {
        out = std::format_to(out, "{}", sp[i]);
      }

      if (i + 1 < sp.size()) { out = std::format_to(out, ", "); }
    }

    return std::format_to(out, "]");
  }
};
