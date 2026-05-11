/**
 * ULF_COM Base transmission
 *
 * \file    src/libklug/internal/transmission/com/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/com/base.hpp"
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
Base::Base(std::shared_ptr<Connection> conn,
           std::string payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, timeout} {}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 */
Base::Base(std::shared_ptr<Connection> conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, timeout} {}

/**
 * Evaluate
 *
 * \return result_t Result
 * \todo Make a ping struct
 */
res::Result Base::evaluate() {
  using std::operator""sv;
  if (std::string_view{std::bit_cast<char const*>(_payload.data()),
                       _payload.size()} == "PING\r"sv) {
    return res::String{std::string_view{
      reinterpret_cast<char const*>(_response.data()), _response.size()}};
  }

  // Everything else is just bool
  return res::Status{
    std::string_view{std::bit_cast<char const*>(_response.data()),
                     _response.size()} == "OK\r"sv};
}

} // namespace transmission::com
