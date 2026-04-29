#pragma once

#include "callback.hpp"
#include "connection.hpp"
#include "transmission.hpp"

namespace transmission {

struct COMTransmission : public Transmission {
  COMTransmission(Connection& conn, std::string payload, std::size_t timeout);
  COMTransmission(Connection& conn,
                  std::span<uint8_t const> payload,
                  std::size_t timeout);

  virtual result_t evaluate() override;
};

}  // namespace transmission