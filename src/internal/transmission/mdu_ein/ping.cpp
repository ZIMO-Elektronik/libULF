/**
 * MDU_EIN Ping
 *
 * \file    src/internal/transmission/mdu_ein/ping.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "internal/transmission/mdu_ein/ping.hpp"
#include <ulf/mdu_ein.hpp>

namespace transmission::mdu_ein {

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Ping::Ping(Connection& conn, std::string payload, std::size_t timeout)
  : Base{conn, payload, timeout} {}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Ping::Ping(Connection& conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : Base{conn, payload, timeout} {}

/**
 * Evaluate Ping response
 *
 * \return result_t Result
 */
result_t Ping::evaluate() {
  result_t r{};

  if (!valid()) {
    r.type = result_type::error;
    r.data.error = -1;
  } else {
    r.type = result_type::status;
    r.data.success =
      (_response[0] == ulf::mdu_ein::ack && _response[2] == ulf::mdu_ein::nak);
  }
  return r;
}

}  // namespace transmission::mdu_ein