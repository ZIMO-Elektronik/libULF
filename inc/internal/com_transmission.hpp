#pragma once

#include "transmission.hpp"

namespace transmission {

struct COMTransmission : public Transmission {
  COMTransmission(std::string payload, std::size_t timeout);
  COMTransmission(std::span<const uint8_t> payload, std::size_t timeout);

  virtual bool evaluate() override;
};

}  // namespace transmission