/**
 * Transmission interface
 *
 * \file    inc/internal/transmission/i_transmission.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "libklug/result/result.hpp"

namespace transmission {

/**
 * Transmission Interface
 *
 * \details
 * Inheriting transmissions can be executed using \ref ITransmission::execute.
 * This will transmit and receive ONCE and produce a result. The result can be
 * extracted using \ref ITransmission::evaluate.
 *
 * \note
 * Per design of the ULF_COM protocol, it is recommended to have only one active
 * transmission. Multiple parallel transmissions WILL result in UB
 */
struct ITransmission {
  virtual ~ITransmission() = default;

  virtual int execute() = 0;

  virtual res::Result evaluate() = 0;
};

} // namespace transmission
