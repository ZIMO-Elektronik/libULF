#pragma once

#include "result.hpp"

namespace transmission {

struct ITransmission {
  virtual int execute() = 0;

  virtual result_t evaluate() = 0;
};

}  // namespace transmission