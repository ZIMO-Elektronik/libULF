/**
 * MDU_EIN ping transmission
 *
 * \file    inc/libklug/internal/transmission/mdu_ein/ping.hpp
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
  Ping(std::shared_ptr<internal::IConnection> conn,
       std::string payload,
       std::size_t timeout);
  Ping(std::shared_ptr<internal::IConnection> conn,
       std::span<uint8_t const> payload,
       std::size_t timeout);
  virtual ~Ping() = default;

  virtual bool evaluateBool() override;
};

} // namespace transmission::mdu_ein
