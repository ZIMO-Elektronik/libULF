/**
 * Copyright (C) 2026 ZIMO Elektronik
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 *
 *
 *
 *
 *
 * USB transmission base
 *
 * \file    src/transmission/transmission_base.hpp
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
#include "connection/i_connection.hpp"
#include "i_transmission.hpp"

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
  virtual int evaluateValue() override;

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
