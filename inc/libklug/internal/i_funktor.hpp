/**
 * Funktor Interface
 *
 * \file    inc/internal/i_funktor.hpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include "libklug/callback.hpp"

namespace internal {

/**
 * Funktor Interface
 *
 * \note This interface should allow to handle platform specifics when handling
 * callbacks
 */
struct IFunktor {
  virtual ~IFunktor() = default;

  virtual void operator()(result_t const& r) = 0;
};

} // namespace internal
