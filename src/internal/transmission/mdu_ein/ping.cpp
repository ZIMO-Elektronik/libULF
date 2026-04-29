#include "internal/transmission/mdu_ein/ping.hpp"
#include <ulf/mdu_ein.hpp>

namespace transmission::mdu_ein {

Ping::Ping(Connection& conn, std::string payload, std::size_t timeout)
  : Base{conn, payload, timeout} {}
Ping::Ping(Connection& conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : Base{conn, payload, timeout} {}

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