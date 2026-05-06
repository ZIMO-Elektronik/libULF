/**
 * COM base transmission
 *
 * \file    inc/internal/transmission/com/base.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "libklug/internal/transmission/transmission_base.hpp"

namespace transmission::com {

/**
 * COM base transmission
 *
 * \internal Overrides \ref Base::evaluate
 *
 */
struct Base : public TransmissionBase {
  Base(Connection& conn, std::string payload, std::size_t timeout);
  Base(Connection& conn, std::span<uint8_t const> payload, std::size_t timeout);

  virtual ~Base() = default;

  virtual res::Result evaluate() override;
};

} // namespace transmission::com
