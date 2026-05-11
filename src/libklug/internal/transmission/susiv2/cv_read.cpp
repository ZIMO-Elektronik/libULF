/**
 * SUSIV2 CvRead
 *
 * \file    src/internal/transmission/susiv2/cv_read.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/susiv2/cv_read.hpp"
#include <ulf/susiv2.hpp>
#include <zusi/utility.hpp>

namespace transmission::susiv2 {

/**
 * CTor
 *
 * \param conn    Connection
 * \param timeout Timeout
 * \param cv      Cv address
 */
CvRead::CvRead(std::shared_ptr<Connection> conn, size_t timeout, uint16_t cv)
  : Base{conn,
         ulf::susiv2::packet2frame<
           ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
           zusi::make_cv_read_packet(0, cv)),
         timeout} {}

/**
 * Evaluate
 *
 * \return result_t Result
 * \todo Insert real error
 */
res::Result CvRead::evaluate() {
  if (!valid()) { return res::Error{err::Error::format}; }

  /// \todo Evaluate crc
  return res::Cv{_response[1u]};
}

} // namespace transmission::susiv2
