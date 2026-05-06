/**
 * Functor Interface
 *
 * \file    inc/libklug/callback/i_functor.hpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include "callback.h"
#include "libklug/result/result.hpp"

namespace internal {

/**
 * Functor Interface
 *
 * \note This interface should allow to handle platform specifics when handling
 * callbacks
 */
struct IFunctor {
  virtual ~IFunctor() = default;

  virtual void operator()(res::Result const& r) = 0;
};

} // namespace internal
