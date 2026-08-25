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
#include <optional>
#include <span>
#include <string>
#include <vector>
#include "i_transmission.hpp"
#include "libklug/callback/callback.h"
#include "libklug/internal/connection/i_connection.hpp"

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
  TransmissionBase(std::shared_ptr<internal::IConnection> conn,
                   std::string_view payload,
                   std::size_t timeout);
  TransmissionBase(std::shared_ptr<internal::IConnection> conn,
                   std::span<uint8_t const> payload,
                   std::size_t timeout);
  TransmissionBase(std::shared_ptr<internal::IConnection> conn,
                   std::string_view payload,
                   uint8_t terminator,
                   std::size_t timeout);
  TransmissionBase(std::shared_ptr<internal::IConnection> conn,
                   std::span<uint8_t const> payload,
                   uint8_t termnator,
                   std::size_t timeout);
  virtual ~TransmissionBase() = default;

  virtual void execute() override;

  virtual std::string evaluateString() override;
  virtual bool evaluateBool() override;
  virtual uint8_t evaluateByte() override;

protected:
  void transmit();
  void receive();

  std::vector<uint8_t> _payload;  ///< Payload
  std::vector<uint8_t> _response; ///< Response buffer
  std::size_t _timeout;           ///< Timeout

  std::optional<uint8_t> _terminator{};         ///< Terminator
  std::shared_ptr<internal::IConnection> _conn; ///< USB Connection
};

} // namespace transmission
