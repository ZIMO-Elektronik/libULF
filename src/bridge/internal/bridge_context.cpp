#include "bridge/internal/bridge_context.hpp"

namespace bridge {

bool Context::transmission(transmission::TransmissionBase* t) {
  mut_transmission.lock();
  bool r{!_transmission};
  if (r) _transmission = t;
  mut_transmission.unlock();
  return r;
}

transmission::TransmissionBase* Context::transmission() {
  return _transmission;
}

}  // namespace bridge