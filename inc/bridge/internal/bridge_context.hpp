#pragma once

#include <future>
#include <mutex>
#include "callback.hpp"
#include "internal/connection.hpp"
#include "internal/transmission/transmission_base.hpp"

namespace bridge {

/**
 * Bridge context
 *
 * \details Holds the current transmission state, as well as the Connection info
 *
 * \todo Maybe construct a state of connection to use later on.
 *
 */
struct Context {

  Connection connection;  ///< Libusb connection info

  bool valid() const;
  bool transmission(transmission::TransmissionBase* t);
  transmission::TransmissionBase* transmission();
  bridge_callback cb;            ///< Callback
  std::future<result_t> result;  ///< Last result

private:
  std::mutex mut_transmission;                    ///< Transmission mutex
  transmission::TransmissionBase* _transmission;  ///< Current transmission
};

}  // namespace bridge