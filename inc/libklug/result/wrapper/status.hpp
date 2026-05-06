/**
 * Status result wraper
 *
 * \file    inc/libklug/result/wrapper/status.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <cstdint>

namespace res {

/**
 * Status wrapper
 *
 * \details Wrapper to provide the result of any command that either succeeds or
 * fails. An example would be a MDU_EIN ping.
 *
 */
struct Status {
  constexpr Status() = default;
  constexpr Status(bool v) : _value{v} {}
  constexpr Status(Status const& e) = default;
  constexpr ~Status() = default;

  constexpr operator bool() const { return _value; }
  constexpr bool operator==(bool v) const { return _value == v; }
  constexpr void operator=(bool v) { _value = v; }
  constexpr void operator=(Status const& lhs) { _value = lhs._value; }

private:
  bool _value{};
};

} // namespace res
