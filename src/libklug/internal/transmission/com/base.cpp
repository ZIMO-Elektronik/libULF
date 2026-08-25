/**
 * ULF_COM Base transmission
 *
 * \file    src/libklug/internal/transmission/com/base.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/transmission/com/base.hpp"
#include <utility>
#include "libklug/internal/exception/e_generic.hpp"
#include "libklug/internal/logging.hpp"

std::array<char, 64> tmp_buffer;

namespace transmission::com {

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 *
 * \todo Put terminator behind constant
 */
Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::string payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, '\r', timeout} {}

/**
 * CTor
 *
 * \param conn    Connection
 * \param payload Payload
 * \param timeout Timeout
 *
 * \todo Put terminator behind constant
 */
Base::Base(std::shared_ptr<internal::IConnection> conn,
           std::span<uint8_t const> payload,
           std::size_t timeout)
  : TransmissionBase{conn, payload, 'r', timeout} {}

/**
 * Evaluate a string
 *
 * \retval std::string          Evaluated bool
 * \retval err::Error::format   Format mismatch
 */
std::string Base::evaluateString() {
  return std::string{reinterpret_cast<char const*>(_response.data()),
                     _response.size()};
}

/**
 * Evaluate a bool
 *
 * \retval bool                 Evaluated bool
 * \retval err::Error::format   Format mismatch
 */
bool Base::evaluateBool() {
  using std::operator""sv;
  if (std::string_view{std::bit_cast<char const*>(_response.data()),
                       _response.size()} == "OK\r"sv)
    return true;
  else if (std::string_view{std::bit_cast<char const*>(_response.data()),
                            _response.size()} == "NOT_OK\r"sv)
    return false;

  throw except::generic_error{err::Error::format, "Format mismatch"sv};
  std::unreachable();
}

} // namespace transmission::com
