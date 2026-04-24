#include "internal/com_transmission.hpp"

namespace transmission {
COMTransmission::COMTransmission(std::string payload, std::size_t timeout)
  : Transmission{payload, timeout} {}
COMTransmission::COMTransmission(std::span<uint8_t const> payload,
                                 std::size_t timeout)
  : Transmission{payload, timeout} {}

bool COMTransmission::evaluate() {
  return this->_response.back() == std::bit_cast<uint8_t>('\r');
}

}  // namespace transmission