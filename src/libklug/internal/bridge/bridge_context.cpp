/**
 * Bridge context
 *
 * \file    src/libklug/internal/bridge/bridge_context.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_context.hpp"
#include "libklug/internal/connection/libserialport_connection.hpp"
#include "libklug/internal/connection/libusb_connection.hpp"

namespace bridge {

/**
 * CTor
 *
 */
Context::Context()
  : connection{std::make_shared<internal::LibserialportConnection>()} {}

/**
 * CTor
 *
 * \param conn Connection ptr
 */
Context::Context(std::shared_ptr<internal::IConnection> conn)
  : connection{conn} {}

/**
 * Setter for transmission
 *
 * \note Only sets transmission, if no transmission is present
 *
 * \param t Transmission
 * \return true   Success
 * \return false  Busy
 */
bool Context::transmission(transmission::TransmissionBase* t) {
  mut_transmission.lock();
  bool r{!_transmission};
  if (r) _transmission = t;
  mut_transmission.unlock();
  return r;
}

/**
 * Getter for transmission
 *
 * \return transmission::TransmissionBase* transmission
 */
transmission::TransmissionBase* Context::transmission() {
  return _transmission;
}

} // namespace bridge
