/**
 * MDU_EIN Ping
 *
 * \file    src/libklug/internal/transmission/mdu_ein/ping.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/mdu_ein/ping.hpp"
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
Ping::Ping(std::shared_ptr<Connection> conn,
           std::string payload,
           std::size_t timeout)
  : Base{conn, payload, timeout} {}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Ping::Ping(std::shared_ptr<Connection> conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : Base{conn, payload, timeout} {}

/**
 * Evaluate Ping response
 *
 * \return Result Result
 * \todo Insert a real error code
 */
res::Result Ping::evaluate() {
  if (!valid()) return res::Error{err::Error::format};
  return res::Status{_response[0] == ulf::mdu_ein::ack &&
                     _response[2] == ulf::mdu_ein::nak};
}

} // namespace transmission::mdu_ein
