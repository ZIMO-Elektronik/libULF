/**
 * Libusb_error result wraper
 *
 * \file    inc/libklug/result/wrapper/libusb_error.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cstdint>

namespace res {

/**
 * LibusbError wrapper
 *
 * \note Used on a libusb error
 *
 */
struct LibusbError {
  constexpr LibusbError() = default;
  constexpr LibusbError(int v) : _value{v} {}
  constexpr LibusbError(LibusbError const& e) = default;
  constexpr ~LibusbError() = default;

  constexpr operator int() const { return _value; }
  constexpr bool operator==(int v) const { return _value == v; }
  constexpr void operator=(int v) { _value = v; }
  constexpr void operator=(LibusbError const& lhs) { _value = lhs._value; }

private:
  int _value{};
};

} // namespace res
