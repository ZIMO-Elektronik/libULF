#pragma once

#include "internal/transmission/transmission_base.hpp"

namespace transmission::mdu_ein {

struct Base : public TransmissionBase {
  Base(Connection& conn, std::string payload, std::size_t timeout);
  Base(Connection& conn, std::span<uint8_t const> payload, std::size_t timeout);
  virtual ~Base() = default;

  virtual result_t evaluate() override;

protected:
  bool valid();
};

}  // namespace transmission::mdu_ein