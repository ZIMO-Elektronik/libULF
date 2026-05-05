/**
 * ULF_COM Base transmission
 *
 * \file    src/internal/transmission/com/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/com/base.hpp"
#include "libklug/callback.hpp"
#include "libklug/internal/logging.hpp"

std::array<char, 64> tmp_buffer;

namespace transmission::com {

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
 * Evaluate
 *
 * \return result_t Result
 */
result_t Base::evaluate() {
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

} // namespace transmission::com
