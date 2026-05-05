#include "internal/funktor.hpp"

namespace internal {

Funktor::Funktor(bridge_callback cb) : _cb{cb} {}

void Funktor::operator()(result_t const& r) { return _cb(r); }

} // namespace internal
