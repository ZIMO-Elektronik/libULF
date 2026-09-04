/**
 * Copyright (C) 2026 [ZIMO Elektronik]
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
 * KLUG Exception
 *
 * \file    include/klug/cpp/klug_error.hpp
 * \author  Jonas Gahlert
 * \date    04.09.2026
 */

#pragma once

#include <stdexcept>
#include "error.hpp"
#include "klug/c/error.h"

namespace libklug {

/**
 * Generic Exception
 *
 * \note
 * Can either be converted to `res::Error` or `result`
 *
 */
struct klug_error : public std::runtime_error {
  klug_error(Error const& code, char const* what_arg)
    : _code{code}, runtime_error{what_arg} {}
  klug_error(libklug_error const& code, char const* what_arg)
    : _code{static_cast<Error>(code)}, runtime_error{what_arg} {}
  klug_error(klug_error const& other) = default;

  klug_error& operator=(klug_error const&) = default;

  explicit operator Error() const { return _code; }

private:
  Error _code;
};

} // namespace libklug
