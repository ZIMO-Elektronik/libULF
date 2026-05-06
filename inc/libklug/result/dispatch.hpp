/**
 * Result dispatch
 *
 * \file    dispatch.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <variant>
#include "result.h"
#include "result.hpp"

namespace res {

// Overload helper
template<class... Ts>
struct overloads : Ts... {
  using Ts::operator()...;
};
template<class... Ts>
overloads(Ts...) -> overloads<Ts...>;

/**
 * Result dispatch
 *
 * \note Converts a C++ `Result` to a C `result`
 *
 * \param r Result
 * \return result
 */
constexpr result dispatch(Result r) {
  return std::visit(overloads{
                      [](String s) {
                        result r{.type = result_type::string};
                        r.data.string = static_cast<std::string>(s).data();
                        return r;
                      },
                      [](Status s) {
                        result r{.type = result_type::status};
                        r.data.success = s;
                        return r;
                      },
                      [](Cv cv) {
                        result r{.type = result_type::cv};
                        r.data.value = cv;
                        return r;
                      },
                      [](Error e) {
                        result r{.type = result_type::error};
                        r.data.error = e;
                        return r;
                      },
                      [](LibusbError e) {
                        result r{.type = result_type::libusb_error};
                        r.data.libusb_error = e;
                        return r;
                      },
                    },
                    r);
}

} // namespace res
