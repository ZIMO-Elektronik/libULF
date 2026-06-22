/**
 * Result dispatch
 *
 * \file    inc/libklug/result/dispatch.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <utility>
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
                        r.data.string = s->data();
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
                        r.data.error =
                          std::to_underlying(static_cast<err::Error>(e));
                        return r;
                      },
                    },
                    r);
}

/**
 * Converts a C `result` to a C++ `Result`
 *
 * \param r result
 * \return Result
 */
constexpr Result dispatch(result r) {
  switch (r.type) {
    case result_type::status: return Status{r.data.success == LIBKLUG_TRUE};
    case result_type::cv: return Cv{static_cast<uint8_t>(r.data.value)};
    case result_type::string: return String{std::string_view{r.data.string}};
    case result_type::error:
      return Error{static_cast<err::Error>(r.data.error)};
  }
}

} // namespace res
