/**
 * SUSIV2 base transmission
 *
 * \file    inc/internal/transmission/susiv2/base.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <span>
#include "internal/transmission/transmission_base.hpp"

namespace transmission::susiv2 {

/**
 * SUSIV2 base transmission
 *
 * \internal  Overrides \ref Base::evaluate to match response to SUSIV2 specs
 *
 */
struct Base : public TransmissionBase {
  Base(Connection& conn, std::string payload, std::size_t timeout);
  Base(Connection& conn, std::span<uint8_t const> payload, std::size_t timeout);

  virtual ~Base() = default;

  virtual result_t evaluate() override;
};

}  // namespace transmission::susiv2