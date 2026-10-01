/**
 * Copyright (C) 2026 ZIMO Elektronik
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 *
 *
 *
 *
 *
 * Asserter
 *
 * \file    src/log/asserter.hpp
 * \author  Jonas Gahlert
 * \date    04.09.2026
 */

#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "logger.hpp"

namespace internal::log {

// Helper to add a message to an assert
class Asserter {
public:
  Asserter(char const* file, int line, char const* expression) {
    _stream << "[ASSERT FAILED] (" << file << ":" << line << ") Condition '"
            << expression << "' failed. Message: ";
  }

  template<typename T>
  constexpr Asserter& operator<<(T const& value) {
    _stream << value;
    return *this;
  }

  ~Asserter() {
    Logger::get().log(Level::Critical, _stream.str());
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::abort();
  }

private:
  std::stringstream _stream;
};

} // namespace internal::log

#define LIBULF_ASSERT(expr)                                                    \
  if (static_cast<bool>(expr)) [[likely]]                                      \
    void(0);                                                                   \
  else internal::log::Asserter(__FILE__, __LINE__, #expr)
