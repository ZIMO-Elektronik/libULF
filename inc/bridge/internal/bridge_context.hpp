#pragma once

#include <future>
#include <mutex>
#include "callback.hpp"
#include "internal/connection.hpp"
#include "internal/transmission/transmission_base.hpp"

namespace bridge {

struct Context {

  Connection connection;

  bool valid() const;
  bool transmission(transmission::TransmissionBase* t);
  transmission::TransmissionBase* transmission();
  bridge_callback cb;
  std::future<result_t> result;  ///< Last result

private:
  std::mutex mut_transmission;
  transmission::TransmissionBase* _transmission;
};

}  // namespace bridge