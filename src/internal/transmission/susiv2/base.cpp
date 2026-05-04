/**
 * SUSUV2 Base transmission
 *
 * \file    src/internal/transmission/susiv2/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "internal/transmission/susiv2/base.hpp"
#include <ulf/susiv2.hpp>

namespace transmission::susiv2 {

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
 * Evaluate SUSIV2 response
 *
 * \return true   Valid
 * \return false  Invalid
 * \todo refactor
 */
result_t Base::evaluate() {
  result_t r{};
  if (!valid()) {
    r.type = result_type::error;
    r.data.error = -1;
  } else {
    r.type = result_type::status;
    r.data.success = 0u;
  }

  return r;
}

bool Base::valid() {
  return _response.size() >= 1uz && _response.size() <= 6uz &&
         _response.front() == ulf::susiv2::ack;
}

}  // namespace transmission::susiv2