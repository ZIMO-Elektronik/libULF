/**
 * Generic Exception
 *
 * \file    inc/libklug/internal/exception/e_generic.hpp
 * \author  Jonas Gahlert
 * \date    19.05.2026
 */

#pragma once

#include <stdexcept>
#include "libklug/error/error.hpp"
#include "libklug/error/error2string.hpp"

namespace except {

/**
 * Generic Exception
 *
 * \note
 * Can either be converted to `res::Error` or `result`
 *
 */
struct generic_error : public std::exception {
  generic_error(err::Error const& code, std::string const& what_arg)
    : _code{code}, _what{what_arg} {
    _what.append(" ").append(err::error2string(_code));
  }
  generic_error(err::Error const& code, std::string_view const what_arg)
    : _code{code}, _what{what_arg} {
    _what.append(" ").append(err::error2string(_code));
  }
  generic_error(err::Error const& code, char const* what_arg)
    : _code{code}, _what{what_arg} {
    _what.append(" ").append(err::error2string(_code));
  }
  generic_error(generic_error const& other) = default;

  virtual char const* what() const noexcept override { return _what.data(); }

  constexpr generic_error& operator=(generic_error const&) = default;

  constexpr explicit operator err::Error() const { return _code; }

private:
  err::Error const _code;
  std::string _what;
};

} // namespace except
