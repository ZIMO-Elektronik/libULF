/**
 * Libusb Exception
 *
 * \file    inc/libklug/internal/exception/e_libusb.hpp
 * \author  Jonas Gahlert
 * \date    19.05.2026
 */

#pragma once

#include <libusb-1.0/libusb.h>
#include <stdexcept>
#include "libklug/result/dispatch.hpp"
#include "libklug/result/result.h"
#include "libklug/result/result.hpp"

namespace except {

/**
 * Libusb Exception
 *
 * \note
 * Can be converted to either a `res::LibusbError` or a `result`
 *
 */
struct libusb_error : public std::exception {
  libusb_error(int const& code, std::string const& what_arg)
    : _code{code}, _what{what_arg} {
    _what.append(" ").append(libusb_error_name(_code));
  }
  libusb_error(int const& code, std::string_view const what_arg)
    : _code{code}, _what{what_arg} {
    _what.append(" ").append(libusb_error_name(_code));
  }
  libusb_error(int const& code, char const* what_arg)
    : _code{code}, _what{what_arg} {
    _what.append(" ").append(libusb_error_name(_code));
  }
  libusb_error(libusb_error const& other) = default;

  virtual char const* what() const noexcept override { return _what.data(); }

  constexpr libusb_error& operator=(libusb_error const&) = default;

  constexpr explicit operator int() const { return _code; }
  constexpr explicit operator res::LibusbError() const {
    return res::LibusbError{_code};
  }
  constexpr explicit operator result() const {
    return res::dispatch(res::LibusbError{_code});
  }

private:
  int const _code;
  std::string _what;
};

} // namespace except
