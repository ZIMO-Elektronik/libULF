/**
 * USB transmission base
 *
 * \file    inc/libklug/internal/transmission/transmission_base.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <vector>
#include "i_transmission.hpp"
#include "libklug/callback/callback.h"
#include "libklug/internal/connection.hpp"

namespace transmission {

/**
 * Transmission base
 *
 * \details
 * Interface for a single USB transmission, intended to use with the ULF_COM
 * protocol specs
 *
 * For non-standard cases, \ref TransmissionBase::execute can be overridden.
 *
 * \note Since `evaluate` is not implemented, this remains an abstract class
 *
 */
struct TransmissionBase : ITransmission {
  TransmissionBase(std::shared_ptr<Connection> conn,
                   std::string payload,
                   std::size_t timeout);
  TransmissionBase(std::shared_ptr<Connection> conn,
                   std::span<uint8_t const> payload,
                   std::size_t timeout);
  virtual ~TransmissionBase() = default;

  virtual res::Result execute() override;

protected:
  int transmit();
  int receive();

  std::vector<uint8_t> _payload;  ///< Payload
  std::vector<uint8_t> _response; ///< Response buffer
  std::size_t _timeout;           ///< Timeout

  std::shared_ptr<Connection> _conn; ///< USB Connection
  void flush();
};

} // namespace transmission
