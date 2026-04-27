#include "internal/susiv2_transmission.hpp"
#include <ulf/susiv2.hpp>

namespace transmission {

SUSIV2Transmission::SUSIV2Transmission(Connection& conn,
                                       std::string payload,
                                       std::size_t timeout)
  : Transmission{conn, payload, timeout} {}

SUSIV2Transmission::SUSIV2Transmission(Connection& conn,
                                       std::span<uint8_t const> payload,
                                       std::size_t timeout)
  : Transmission{conn, payload, timeout} {}

/**
 * Evaluate SUSIV2 response
 *
 * \return true   Valid
 * \return false  Invalid
 * \todo refactor
 */
bool SUSIV2Transmission::evaluate() {
  if (_response.size() < 1uz || _response.size() > 6uz) return false;
  if (_response.front() != ulf::susiv2::ack) return false;
  return true;
}

}  // namespace transmission