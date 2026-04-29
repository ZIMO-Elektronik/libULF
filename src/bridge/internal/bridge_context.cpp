#include "bridge/internal/bridge_context.hpp"

namespace bridge {

bool Context::transmission(transmission::TransmissionBase* t) {
  mut_transmission.lock();
  bool result{!_transmission};
  if (result) _transmission = t;
  mut_transmission.unlock();
  return result;
}

transmission::TransmissionBase* Context::transmission() {
  return _transmission;
}

}  // namespace bridge