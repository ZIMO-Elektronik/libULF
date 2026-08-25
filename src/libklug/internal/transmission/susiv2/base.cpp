/**
 * SUSUV2 Base transmission
 *
 * \file    src/internal/transmission/susiv2/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/susiv2/base.hpp"
#include <ulf/susiv2.hpp>
#include <utility>
#include "libklug/internal/exception/e_generic.hpp"

namespace transmission::susiv2 {

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
  : TransmissionBase{conn, payload, timeout} {}

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
  : TransmissionBase{conn, payload, timeout} {}

/**
 * Evaluate a bool
 *
 * \retval bool                 Evaluated bool
 * \retval err::Error::format   Format mismatch
 */
bool Base::evaluateBool() {
  using std::operator""sv;
  if (!valid()) {
    throw except::generic_error{err::Error::format, "Format Mismatch"sv};
    std::unreachable();
  }
  return _response.front() == ulf::susiv2::ack;
}

bool Base::valid() {
  return _response.size() >= 1uz && _response.size() <= 6uz &&
         (_response.front() == ulf::susiv2::ack ||
          _response.front() == ulf::susiv2::nak);
}

} // namespace transmission::susiv2
