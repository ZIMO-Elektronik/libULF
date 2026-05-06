/**
 * Cv result wraper
 *
 * \file    inc/libklug/result/wrapper/cv.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cstdint>

namespace res {

/**
 * Cv value wrapper
 *
 * \note Used as a the result of a Cv Read operation
 *
 */
struct Cv {
  constexpr Cv() = default;
  constexpr Cv(uint8_t v) : _value{v} {}
  constexpr Cv(Cv const& e) = default;
  constexpr ~Cv() = default;

  constexpr operator uint8_t() const { return _value; }
  constexpr bool operator==(uint8_t v) const { return _value == v; }
  constexpr void operator=(uint8_t v) { _value = v; }
  constexpr void operator=(Cv const& lhs) { _value = lhs._value; }

private:
  uint8_t _value{};
};

} // namespace res
