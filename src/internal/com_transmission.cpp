#include "internal/com_transmission.hpp"

namespace transmission {
COMTransmission::COMTransmission(Connection& conn,
                                 std::string payload,
                                 std::size_t timeout)
  : Transmission{conn, payload, timeout} {}
COMTransmission::COMTransmission(Connection& conn,
                                 std::span<uint8_t const> payload,
                                 std::size_t timeout)
  : Transmission{conn, payload, timeout} {}

bool COMTransmission::evaluate() {
  return this->_response.back() == std::bit_cast<uint8_t>('\r');
}

}  // namespace transmission