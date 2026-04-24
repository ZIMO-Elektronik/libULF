#include "bridge/internal/bridge.hpp"

namespace bridge {

int Bridge::open(uint16_t vid, uint16_t pid) {
  return _context.connection.open(vid, pid);
}

int Bridge::openFd(int Fd) { return _context.connection.openFd(Fd); }

int Bridge::config() { return _context.connection.config(); }

int Bridge::claim() { return _context.connection.claim(); }

int Bridge::release() { return _context.connection.release(); }

void Bridge::close() { return _context.connection.close(); }

}  // namespace bridge