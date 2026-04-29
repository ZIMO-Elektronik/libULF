#pragma once

#include "base.hpp"

namespace transmission::mdu_ein {

struct Ping : public Base {
  Ping(Connection& conn, std::string payload, std::size_t timeout);
  Ping(Connection& conn, std::span<uint8_t const> payload, std::size_t timeout);
  virtual ~Ping() = default;

  virtual result_t evaluate() override;
};

}  // namespace transmission::mdu_ein
