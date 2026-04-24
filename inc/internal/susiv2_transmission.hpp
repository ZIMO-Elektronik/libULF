#pragma once

#include "transmission.hpp"
#include <span> 

namespace transmission {

struct SUSIV2Transmission : public Transmission {
  SUSIV2Transmission(std::string payload, std::size_t timeout);
  SUSIV2Transmission(std::span<const uint8_t> payload, std::size_t timeout);

  virtual bool evaluate() override;
};

}