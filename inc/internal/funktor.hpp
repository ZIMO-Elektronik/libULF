/**
 * Funktor
 *
 * \file    inc/internal/funktor.hpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include "callback.hpp"
#include "i_funktor.hpp"

namespace internal {

/**
 * Funktor
 *
 * \note Standard funktor with attached callback
 *
 */
struct Funktor : public IFunktor {
  Funktor(bridge_callback cb);
  virtual ~Funktor() = default;

  virtual void operator()(result_t const& r) override;

private:
  bridge_callback _cb; ///< Callback
};

} // namespace internal
