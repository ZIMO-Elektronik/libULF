#include "internal/transmission/mdu_ein/cv_read.hpp"
#include <ulf/mdu_ein.hpp>
#include "internal/transmission/mdu_ein/base.hpp"

namespace transmission::mdu_ein {

CvRead::CvRead(Connection& conn, uint16_t cv) : _conn{conn}, _cv{cv}, _value{0u} {}

/// \todo refactor this (hard)
int CvRead::execute() {
  for (uint8_t i{0}; i < sizeof(_value) * 8u; i++) {
    Base t{_conn,
           ulf::mdu_ein::bytes2mdu_ein(mdu::make_cv_read_packet(_cv, i)),
           100u};
    t.execute();

    auto const r = t.evaluate();

    /// \todo Implement retry for single bits
    if (r.type != result_type::status) return -1;

    _value |= !(r.data.success) << i;
  }
  return 0;
}

result_t CvRead::evaluate() {
  result_t r{.type = result_type::cv};
  r.data.value = _value;
  return r;
}

}  // namespace transmission::mdu_ein