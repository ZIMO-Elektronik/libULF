/**
 * Error result wraper
 *
 * \file    inc/libklug/result/wrapper/error.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cstdint>
#include "libklug/error/error.hpp"

namespace res {

/**
 * Error wrapper
 *
 * \note Used on a non-libusb error
 *
 */
struct Error {
  constexpr Error() = default;
  constexpr Error(err::Error v) : _value{v} {}
  constexpr Error(Error const& e) = default;
  constexpr ~Error() = default;

  constexpr operator err::Error() const { return _value; }
  constexpr bool operator==(Error const&) const = default;
  constexpr bool operator==(err::Error v) const { return _value == v; }
  constexpr void operator=(err::Error v) { _value = v; }
  constexpr void operator=(Error const& lhs) { _value = lhs._value; }

private:
  err::Error _value{};
};

} // namespace res
