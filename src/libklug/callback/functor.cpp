/**
 * Functor
 *
 * \file    src/libklug/callback/functor.cpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#include "libklug/callback/functor.hpp"
#include "libklug/result/dispatch.hpp"

namespace callback {

Functor::Functor(bridge_callback cb) : _cb{cb} {}

void Functor::operator()(res::Result const& r) { return _cb(res::dispatch(r)); }

} // namespace callback
