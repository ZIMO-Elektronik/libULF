/**
 * MDU_EIN Cv Read
 *
 * \file    src/libklug/internal/transmission/mdu_ein/cv_read.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/mdu_ein/cv_read.hpp"
#include <format>
#include <ulf/mdu_ein.hpp>
#include <utility>
#include "libklug/internal/exception/e_generic.hpp"
#include "libklug/internal/transmission/mdu_ein/base.hpp"

namespace transmission::mdu_ein {

/**
 * CTor
 *
 * \param conn  Connection
 * \param cv    Cv address
 */
CvRead::CvRead(std::shared_ptr<internal::IConnection> conn, uint16_t cv)
  : _conn{conn}, _cv{cv}, _value{0u} {}

/**
 * Execute
 *
 * \return int 0
 *
 * \todo Refactor, to avoid exception abuse and to retry single bits
 */
void CvRead::execute() {
  for (uint8_t i{0}; i < sizeof(_value) * 8u; i++) {
    Base t{_conn,
           ulf::mdu_ein::bytes2mdu_ein(mdu::make_cv_read_packet(_cv, i)),
           100u};
    try {
      t.execute();
      _value |= !(t.evaluateBool()) << i;

    } catch (except::generic_error e) {
      throw except::generic_error{
        static_cast<err::Error>(e),
        std::format("CvRead error at bit {}, Cause: {}", i, e.what())};
    }
  }
}

/// Stub
std::string CvRead::evaluateString() {
  using std::operator""sv;
  throw except::generic_error{err::Error::unknown, "Missing Implementation"sv};
  std::unreachable();
}

/// Stub
bool CvRead::evaluateBool() {
  using std::operator""sv;
  throw except::generic_error{err::Error::unknown, "Missing Implementation"sv};
  std::unreachable();
}

uint8_t CvRead::evaluateByte() { return _value; }

} // namespace transmission::mdu_ein
