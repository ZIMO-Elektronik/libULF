/**
 * SUSIV2 CvRead
 *
 * \file    src/internal/transmission/susiv2/cv_read.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/susiv2/cv_read.hpp"
#include <ulf/susiv2.hpp>
#include <utility>
#include <zusi/utility.hpp>
#include "libklug/internal/exception/e_generic.hpp"

namespace transmission::susiv2 {

/**
 * CTor
 *
 * \param conn    Connection
 * \param timeout Timeout
 * \param cv      Cv address
 */
CvRead::CvRead(std::shared_ptr<internal::IConnection> conn,
               size_t timeout,
               uint16_t cv)
  : Base{conn,
         ulf::susiv2::packet2frame<
           ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
           zusi::make_cv_read_packet(0, cv)),
         timeout} {}

/**
 * Evaluate a byte
 *
 * \retval uint8_t              Evaluated byte
 * \retval err::Error::format   Format mismatch
 */
uint8_t CvRead::evaluateByte() {
  using std::operator""sv;
  if (!valid()) {
    throw except::generic_error{err::Error::format, "Format Mismatch"sv};
    std::unreachable();
  }
  return _response[1u];
}

} // namespace transmission::susiv2
