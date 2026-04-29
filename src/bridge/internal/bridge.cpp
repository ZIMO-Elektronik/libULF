#include "bridge/internal/bridge.hpp"

namespace bridge {

int Bridge::init() {
#ifdef ANDROID
  // We cant search devices on Android
  libusb_set_option(NULL, LIBUSB_OPTION_WEAK_AUTHORITY);
  libusb_set_option(nullptr, LIBUSB_OPTION_NO_DEVICE_DISCOVERY);
#endif
  return libusb_init(nullptr);
}

void Bridge::registerCB(bridge_callback cb) { _ctx.cb = cb; }

result_t Bridge::result() {
  _ctx.result.wait();
  return _ctx.result.get();
}

int Bridge::open(uint16_t vid, uint16_t pid) {
  return _ctx.connection.open(vid, pid);
}

int Bridge::openFd(int Fd) { return _ctx.connection.openFd(Fd); }

int Bridge::config() { return _ctx.connection.config(); }

int Bridge::claim() { return _ctx.connection.claim(); }

int Bridge::release() { return _ctx.connection.release(); }

void Bridge::close() { return _ctx.connection.close(); }

COM& Bridge::com() { return _com; }

SUSIV2& Bridge::susiv2() { return _susiv2; }

MDU_EIN& Bridge::mdu_ein() { return _mdu_ein; }

}  // namespace bridge