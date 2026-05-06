/**
 * Error result wraper
 *
 * \file    inc/libklug/result/wrapper/error.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cstdint>

namespace res {

/**
 * Error wrapper
 *
 * \note Used on a non-libusb error
 *
 */
struct Error {
  constexpr Error() = default;
  constexpr Error(int v) : _value{v} {}
  constexpr Error(Error const& e) = default;
  constexpr ~Error() = default;

  constexpr operator int() const { return _value; }
  constexpr bool operator==(int v) const { return _value == v; }
  constexpr void operator=(int v) { _value = v; }
  constexpr void operator=(Error const& lhs) { _value = lhs._value; }

private:
  int _value{};
};

} // namespace res
