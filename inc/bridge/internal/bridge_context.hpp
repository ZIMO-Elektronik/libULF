#pragma once

#include <future>
#include <mutex>
#include "callback.hpp"
#include "internal/connection.hpp"
#include "internal/transmission.hpp"

namespace bridge {

struct Context {

  Connection connection;

  bool valid() const;
  bool transmission(transmission::Transmission* t);
  transmission::Transmission* transmission();
  bridge_callback cb;
  std::future<result_t> result;  ///< Last result

private:
  std::mutex mut_transmission;
  transmission::Transmission* _transmission;
};

}  // namespace bridge