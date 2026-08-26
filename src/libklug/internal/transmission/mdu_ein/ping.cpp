/**
 * MDU_EIN Ping
 *
 * \file    src/libklug/internal/transmission/mdu_ein/ping.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/mdu_ein/ping.hpp"
#include <ulf/mdu_ein.hpp>
#include <utility>
#include "libklug/internal/exception/e_generic.hpp"

namespace transmission::mdu_ein {

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Ping::Ping(std::shared_ptr<internal::IConnection> conn,
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
Ping::Ping(std::shared_ptr<internal::IConnection> conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : Base{conn, payload, timeout} {}

/**
 * Evaluate a bool
 *
 * \note
 * This is the inverse operation, since a decoder naks when it responds
 *
 * \throw generic_error When format does not match protocol
 *
 * \retval Evaluated bool
 */
bool Ping::evaluateBool() {
  using std::operator""sv;
  if (!valid()) {
    throw except::generic_error{err::Error::format, "Format Mismatch"sv};
    std::unreachable();
  }
  return _response[0] == ulf::mdu_ein::ack && _response[2] == ulf::mdu_ein::nak;
}

} // namespace transmission::mdu_ein
