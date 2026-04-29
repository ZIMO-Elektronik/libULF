#pragma once

#include <span>
#include "internal/transmission/transmission_base.hpp"

namespace transmission::susiv2 {

struct Base : public TransmissionBase {
  Base(Connection& conn, std::string payload, std::size_t timeout);
  Base(Connection& conn, std::span<uint8_t const> payload, std::size_t timeout);

  virtual result_t evaluate() override;
};

}  // namespace transmission::susiv2