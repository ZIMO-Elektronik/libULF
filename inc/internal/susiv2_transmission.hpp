#pragma once

#include <span>
#include "transmission.hpp"

namespace transmission {

struct SUSIV2Transmission : public Transmission {
  SUSIV2Transmission(Connection& conn,
                     std::string payload,
                     std::size_t timeout);
  SUSIV2Transmission(Connection& conn,
                     std::span<uint8_t const> payload,
                     std::size_t timeout);

  virtual void push() override {}

  virtual bool evaluate() override;
};

}  // namespace transmission