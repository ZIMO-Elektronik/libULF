/**
 * MDU_EIN Base transmission
 *
 * \file    src/internal/transmission/mdu_ein/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/mdu_ein/base.hpp"
#include <libklug/result/wrapper/error.hpp>
#include <libklug/result/wrapper/status.hpp>
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
 * \return Result Result
 * \todo Insert a real error code
 */
res::Result Base::evaluate() {
  if (!valid()) return res::Error{0};
  return res::Status{_response[0] == ulf::mdu_ein::ack &&
                     _response[2] == ulf::mdu_ein::ack};
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
