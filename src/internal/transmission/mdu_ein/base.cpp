#include "internal/transmission/mdu_ein/base.hpp"
#include <ulf/mdu_ein.hpp>
namespace transmission::mdu_ein {

Base::Base(Connection& conn, std::string payload, std::size_t timeout)
  : TransmissionBase{conn, payload, timeout} {}
Base::Base(Connection& conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, timeout} {}

result_t Base::evaluate() {
  result_t r{};

  if (!valid()) {
    r.type = result_type::error;
    r.data.error = -1;
  } else {
    r.type = result_type::status;
    r.data.success =
      (_response[0] == ulf::mdu_ein::ack && _response[2] == ulf::mdu_ein::ack);
  }

  return r;
}

bool Base::valid() {
  return !(_response.size() != 4uz || _response[1] != ulf::mdu_ein::separator ||
           _response[3] != ulf::mdu_ein::end);
}

}  // namespace transmission::mdu_ein