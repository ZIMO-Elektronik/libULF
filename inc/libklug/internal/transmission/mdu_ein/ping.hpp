/**
 * MDU_EIN ping transmission
 *
 * \file    inc/internal/transmission/mdu_ein/ping.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "base.hpp"

namespace transmission::mdu_ein {

/**
 * MDU_EIN ping transmission
 *
 * \internal Overrides \ref Ping::evaluate since ping success is NAK'ed
 */
struct Ping : public Base {
  Ping(Connection& conn, std::string payload, std::size_t timeout);
  Ping(Connection& conn, std::span<uint8_t const> payload, std::size_t timeout);
  virtual ~Ping() = default;

  virtual res::Result evaluate() override;
};

} // namespace transmission::mdu_ein
