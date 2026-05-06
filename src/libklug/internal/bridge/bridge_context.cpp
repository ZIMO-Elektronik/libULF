/**
 * Bridge context
 *
 * \file    src/libklug/internal/bridge/bridge_context.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_context.hpp"

namespace bridge {

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
