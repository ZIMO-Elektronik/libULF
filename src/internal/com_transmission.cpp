#include "internal/com_transmission.hpp"
#include "callback.hpp"
#include "internal/logging.hpp"

std::array<char, 64> tmp_buffer;

namespace transmission {
COMTransmission::COMTransmission(Connection& conn,
                                 std::string payload,
                                 std::size_t timeout)
  : Transmission{conn, payload, timeout} {}
COMTransmission::COMTransmission(Connection& conn,
                                 std::span<uint8_t const> payload,
                                 std::size_t timeout)
  : Transmission{conn, payload, timeout} {}

result_t COMTransmission::evaluate() {
  using std::operator""sv;
  result_t r{};
  LOGD("Creating result");

  std::string_view str{std::bit_cast<char const*>(_payload.data()),
                       _payload.size()};
  if (str == "PING\r"sv) {
    // Ping results in a string
    r.type = result_type::string;
    std::fill(tmp_buffer.begin(), tmp_buffer.end(), 0);
    std::copy(_response.begin(), _response.end(), tmp_buffer.begin());

    r.data.string = reinterpret_cast<char const*>(tmp_buffer.data());
  } else {
    // Everything else is just bool
    std::string_view re{std::bit_cast<char const*>(_response.data()),
                        _response.size()};
    r.type = result_type::status;
    r.data.success = (re == "OK\r"sv) ? 0 : 1;
  }

  return r;
}

}  // namespace transmission