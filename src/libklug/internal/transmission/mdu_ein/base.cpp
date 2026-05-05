/**
 * MDU_EIN Base transmission
 *
 * \file    src/internal/transmission/mdu_ein/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/mdu_ein/base.hpp"
#include <ulf/mdu_ein.hpp>
namespace transmission::mdu_ein {

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Base::Base(Connection& conn, std::string payload, std::size_t timeout)
  : TransmissionBase{conn, payload, timeout} {}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Base::Base(Connection& conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, timeout} {}

/**
 * Evaluate
 *
 * \return result_t Result
 */
result_t Base::evaluate() {
  result_t r{};

  if (!valid()) {
    r.type = result_type::error;
    r.data.error = -1;
  } else {
    r.type = result_type::status;
    r.data.success =
      (_response[0] == ulf::mdu_ein::ack && _response[2] == ulf::mdu_ein::ack);
  }

  return r;
}

/**
 * Response valid
 *
 * \return true   Valid
 * \return false  Invalid
 */
bool Base::valid() {
  return !(_response.size() != 4uz || _response[1] != ulf::mdu_ein::separator ||
           _response[3] != ulf::mdu_ein::end);
}

} // namespace transmission::mdu_ein
