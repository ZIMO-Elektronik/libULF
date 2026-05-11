/**
 * Internal bridge context
 *
 * \file    inc/libklug/internal/bridge/bridge_context.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <future>
#include <mutex>
#include "libklug/callback/i_functor.hpp"
#include "libklug/internal/connection.hpp"
#include "libklug/internal/transmission/transmission_base.hpp"

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
  Context();
  Context(std::shared_ptr<Connection> conn);

  std::shared_ptr<Connection> connection; ///< Libusb connection info

  bool valid() const;
  bool transmission(transmission::TransmissionBase* t);
  transmission::TransmissionBase* transmission();
  std::unique_ptr<callback::IFunctor> cb; ///< Callback
  std::future<res::Result> result;        ///< Last result

private:
  std::mutex mut_transmission;                   ///< Transmission mutex
  transmission::TransmissionBase* _transmission; ///< Current transmission
};

} // namespace bridge
