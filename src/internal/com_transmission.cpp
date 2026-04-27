#include "internal/com_transmission.hpp"
#include "callback.hpp"
#include "internal/logging.hpp"

namespace transmission {
COMTransmission::COMTransmission(Connection& conn,
                                 bridge_callback cb,
                                 std::string payload,
                                 std::size_t timeout)
  : Transmission{conn, payload, timeout}, _cb{cb} {}
COMTransmission::COMTransmission(Connection& conn,
                                 bridge_callback cb,
                                 std::span<uint8_t const> payload,
                                 std::size_t timeout)
  : Transmission{conn, payload, timeout}, _cb{cb} {}

void COMTransmission::push() {
  using std::operator""sv;
  result_t r{};
  LOGD("Pushing result");

  std::string_view str{std::bit_cast<char const*>(_payload.data()),
                       _payload.size()};
  if (str == "PING\r"sv) {
    // Ping results in a string
    r.type = result_type::string;
    r.data.string = reinterpret_cast<char const*>(_response.data());
  } else {
    // Everything else is just bool
    std::string_view re{std::bit_cast<char const*>(_response.data()),
                        _response.size()};
    r.type = result_type::status;
    r.data.success = (re == "OK\r"sv) ? 0 : 1;
  }
  if (_cb) _cb(r);

  return;
}

bool COMTransmission::evaluate() {
  return this->_response.back() == std::bit_cast<uint8_t>('\r');
}

}  // namespace transmission