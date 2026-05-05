/**
 * Functor
 *
 * \file    functor.cpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#include "libklug/callback/functor.hpp"

namespace internal {

Functor::Functor(bridge_callback cb) : _cb{cb} {}

void Functor::operator()(result_t const& r) { return _cb(r); }

} // namespace internal
