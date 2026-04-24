#pragma once

#include <mutex>
#include "internal/connection.hpp"
#include "internal/transmission.hpp"

namespace bridge {

struct Context {

  Connection connection;
  std::mutex mut_transmission;

  bool valid() const;
  bool transmission(transmission::Transmission* t);
  transmission::Transmission const* transmission() const;

private:
  transmission::Transmission* _transmission;
};

}  // namespace bridge