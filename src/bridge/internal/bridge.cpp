#include "bridge/internal/bridge.hpp"

namespace bridge {

int Bridge::init() { return libusb_init(nullptr); }

void Bridge::registerCB(bridge_callback cb) { _ctx.cb = cb; }

int Bridge::open(uint16_t vid, uint16_t pid) {
  return _ctx.connection.open(vid, pid);
}

int Bridge::openFd(int Fd) { return _ctx.connection.openFd(Fd); }

int Bridge::config() { return _ctx.connection.config(); }

int Bridge::claim() { return _ctx.connection.claim(); }

int Bridge::release() { return _ctx.connection.release(); }

void Bridge::close() { return _ctx.connection.close(); }

COM& Bridge::com() { return _com; }

}  // namespace bridge