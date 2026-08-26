/**
 * MDU_EIN Base transmission
 *
 * \file    src/libklug/internal/transmission/mdu_ein/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/mdu_ein/base.hpp"
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
Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::string payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, ulf::mdu_ein::end, timeout} {}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, ulf::mdu_ein::end, timeout} {}

/**
 * Evaluate a bool
 *
 * \throw generic_error If Format does not match protocol
 *
 * \return Evaluated bool
 */
bool Base::evaluateBool() {
  using std::operator""sv;
  if (!valid()) {
    throw except::generic_error{err::Error::format, "Format mismatch"sv};
    std::unreachable();
  }
  return _response[0] == ulf::mdu_ein::ack && _response[2] == ulf::mdu_ein::ack;
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
