/**
 * Functor
 *
 * \file    inc/libklug/callback/functor.hpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include "i_functor.hpp"
#include "libklug/callback/callback.h"

namespace internal {

/**
 * Funktor
 *
 * \note Standard funktor with attached callback
 *
 */
struct Functor : public IFunctor {
  Functor(bridge_callback cb);
  virtual ~Functor() = default;

  virtual void operator()(result_t const& r) override;

private:
  bridge_callback _cb; ///< Callback
};

} // namespace internal
