#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>
#include "callback.hpp"
#include "i_transmission.hpp"
#include "internal/connection.hpp"

namespace transmission {

struct TransmissionBase : ITransmission {
  TransmissionBase(Connection& conn, std::string payload, std::size_t timeout);
  TransmissionBase(Connection& conn,
                   std::span<uint8_t const> payload,
                   std::size_t timeout);

  virtual int execute();

protected:
  int transmit();
  int receive();

  // virtual bool evaluate() = 0;
  // virtual void notify() = 0;

  std::vector<uint8_t> _payload;
  std::vector<uint8_t> _response;
  std::size_t _timeout;

  Connection& _conn;

  void flush();
};

}  // namespace transmission