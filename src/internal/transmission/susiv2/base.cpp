#include "internal/transmission/susiv2/base.hpp"
#include <ulf/susiv2.hpp>

namespace transmission::susiv2 {

Base::Base(Connection& conn, std::string payload, std::size_t timeout)
  : TransmissionBase{conn, payload, timeout} {}

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
  if (_response.size() < 1uz || _response.size() > 6uz ||
      _response.front() != ulf::susiv2::ack) {
    r.type = result_type::error;
    r.data.error = -1;
  } else {
    auto const cmd{static_cast<zusi::Command>(_payload[5uz])};
    switch (cmd) {
      case zusi::Command::CvRead: {
        r.type = result_type::cv;
        r.data.value = _response[1u];
        break;
      }
      default: {
        r.type = result_type::status;
        r.data.success = 0u;
        break;
      }
    }
  }

  return r;
}

}  // namespace transmission::susiv2