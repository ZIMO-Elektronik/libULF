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
 * ULF Exception
 *
 * \file    include/ulf/cpp/ulf_error.hpp
 * \author  Jonas Gahlert
 * \date    04.09.2026
 */

#pragma once

#include <stdexcept>
#include "error.hpp"
#include "ulf/c/error.h"

namespace libulf {

/**
 * Generic Exception
 *
 * \note
 * Can either be converted to `res::Error` or `result`
 *
 */
struct ulf_error : public std::runtime_error {
  ulf_error(Error const& code, char const* what_arg)
    : _code{code}, runtime_error{what_arg} {}
  ulf_error(libulf_error const& code, char const* what_arg)
    : _code{static_cast<Error>(code)}, runtime_error{what_arg} {}
  ulf_error(ulf_error const& other) = default;

  ulf_error& operator=(ulf_error const&) = default;

  explicit operator Error() const { return _code; }

private:
  Error _code;
};

} // namespace libulf
