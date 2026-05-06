/**
 * String result wraper
 *
 * \file    inc/libklug/result/wrapper/string.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <string>

namespace res {

/**
 * String wrapper
 *
 * \note Used when a command returns a string. An example would be an ULF_COM
 * 'PING\r'
 *
 */
struct String {
  String() = default;
  String(std::string v) : _value{v} {}
  String(String const& e) = default;
  ~String() = default;

  operator std::string() const { return _value; }
  bool operator==(std::string v) const { return _value == v; }
  void operator=(std::string v) { _value = v; }
  void operator=(String const& lhs) { _value = lhs._value; }

private:
  std::string _value{};
};

} // namespace res
