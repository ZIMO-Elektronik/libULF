#pragma once

#include "callback.hpp"
#include "connection.hpp"
#include "transmission.hpp"

namespace transmission {

struct COMTransmission : public Transmission {
  COMTransmission(Connection& conn,
                  bridge_callback cb,
                  std::string payload,
                  std::size_t timeout);
  COMTransmission(Connection& conn,
                  bridge_callback cb,
                  std::span<uint8_t const> payload,
                  std::size_t timeout);

  virtual void push() override;

  virtual bool evaluate() override;

private:
  bridge_callback _cb;
};

}  // namespace transmission