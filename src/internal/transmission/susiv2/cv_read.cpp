/**
 * SUSIV2 CvRead
 *
 * \file    src/internal/transmission/susiv2/cv_read.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "internal/transmission/susiv2/cv_read.hpp"
#include <zusi/utility.hpp>

namespace transmission::susiv2 {

/**
 * CTor
 *
 * \param conn    Connection
 * \param timeout Timeout
 * \param cv      Cv address
 */
CvRead::CvRead(Connection& conn, size_t timeout, uint16_t cv)
  : Base{conn, zusi::make_cv_read_packet(0, cv), timeout} {}

/**
 * Evaluate
 *
 * \return result_t Result
 */
result_t CvRead::evaluate() {
  if (!valid()) {
    result_t r{.type = result_type::error};
    r.data.error = -1;
    return r;
  }

  /// \todo Evaluate crc
  result_t r{.type = result_type::cv};
  r.data.value = _response[1u];
  return r;
}

}  // namespace transmission::susiv2