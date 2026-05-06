/**
 * MDU_EIN Cv Read
 *
 * \file    src/libklug/internal/transmission/mdu_ein/cv_read.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/mdu_ein/cv_read.hpp"
#include <ulf/mdu_ein.hpp>
#include "libklug/internal/transmission/mdu_ein/base.hpp"

namespace transmission::mdu_ein {

/**
 * CTor
 *
 * \param conn  Connection
 * \param cv    Cv address
 */
CvRead::CvRead(std::shared_ptr<Connection> conn, uint16_t cv)
  : _conn{conn}, _cv{cv}, _value{0u} {}

/**
 * Execute
 *
 * \return int 0
 *
 * \todo Refactor
 */
res::Result CvRead::execute() {
  for (uint8_t i{0}; i < sizeof(_value) * 8u; i++) {
    Base t{_conn,
           ulf::mdu_ein::bytes2mdu_ein(mdu::make_cv_read_packet(_cv, i)),
           100u};
    auto res{t.execute()};
    // Check if execution resulted in an error
    if (!std::holds_alternative<res::Status>(res)) return res;

    auto const r = t.evaluate();

    /// \todo Implement retry for single bits
    if (!std::holds_alternative<res::Status>(r)) return r;
    _value |= !(std::get<res::Status>(r)) << i;
  }
  return res::Status{true};
}

/**
 * Evaluate Cv Read
 *
 * \return result_t Result
 */
res::Result CvRead::evaluate() { return res::Cv{_value}; }

} // namespace transmission::mdu_ein
