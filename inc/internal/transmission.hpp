#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>
#include "callback.hpp"
#include "connection.hpp"

namespace transmission {

struct Transmission {
  Transmission(Connection& conn, std::string payload, std::size_t timeout);
  Transmission(Connection& conn,
               std::span<uint8_t const> payload,
               std::size_t timeout);

  int execute();

  std::span<uint8_t> result();

  /// \todo Replace with pure virtual
  virtual result_t evaluate() { return {}; }

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