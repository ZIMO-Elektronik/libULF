/**
 * Internal Bridge config
 *
 * \file    src/libklug/internal/bridge/bridge.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge.hpp"
#include "libklug/internal/exception/e_generic.hpp"
#include "libklug/internal/exception/e_libusb.hpp"

namespace bridge {

Bridge::Bridge(std::shared_ptr<Connection> conn) : _ctx{conn} {}

/**
 * Init USB backend
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 *
 */
int Bridge::init() {
#ifdef ANDROID
  // We can't search devices on Android
  libusb_set_option(NULL, LIBUSB_OPTION_WEAK_AUTHORITY);
  libusb_set_option(nullptr, LIBUSB_OPTION_NO_DEVICE_DISCOVERY);
#endif
  return libusb_init(nullptr);
}

/**
 * Register callback
 *
 * \param cb Callback
 *
 * \todo Replace raw cb with funktor
 */
void Bridge::registerCB(std::unique_ptr<callback::IFunctor> cb) {
  _ctx.cb = std::move(cb);
}

/**
 * Deregister callback
 *
 * \todo Not thread safe, does it neet to be?
 */
void Bridge::deregisterCB() {
  if (_ctx.cb) _ctx.cb.reset();
}

/**
 * Wait and get result
 *
 * \return result_t Result
 *
 * \warning Waiting for a result without a pending operation will wait forever
 *
 * \todo Uhm... Refactor
 */
res::Result Bridge::result() {
  _ctx.result.wait();
  try {
    return _ctx.result.get();
  } catch (except::libusb_error const& e) {
    return static_cast<res::LibusbError>(e);
  } catch (except::generic_error const& e) {
    return static_cast<res::Error>(e);
  }
}

/**
 * Open usb device
 *
 * \param vid   VID
 * \param pid   PID
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int Bridge::open(uint16_t vid, uint16_t pid) {
  return _ctx.connection->open(vid, pid);
}

/**
 * Open usb device by file descriptor
 *
 * \note Recommdended for android
 *
 * \param Fd    File descriptor
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int Bridge::openFd(int Fd) { return _ctx.connection->openFd(Fd); }

/**
 * Configure usb device
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int Bridge::config() { return _ctx.connection->config(); }

/**
 * Claim usb device
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int Bridge::claim() { return _ctx.connection->claim(); }

/**
 * Release usb device
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int Bridge::release() { return _ctx.connection->release(); }

/**
 * Close usb device
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
void Bridge::close() { return _ctx.connection->close(); }

/**
 * COM getter
 *
 * \return COM& COM
 */
COM& Bridge::com() { return _com; }

/**
 * SUSIV2 getter
 *
 * \return SUSIV2& SUSIV2
 */
SUSIV2& Bridge::susiv2() { return _susiv2; }

/**
 * MDU_EIN getter
 *
 * \return MDU_EIN& MDU_EIN
 */
MDU_EIN& Bridge::mdu_ein() { return _mdu_ein; }

/**
 * ZPP getter
 *
 * \return ZPP& ZPP
 */
ZPP& Bridge::zpp() { return _zpp; }

/**
 * ZSU getter
 *
 * \return ZSU&
 */
ZSU& Bridge::zsu() { return _zsu; }

} // namespace bridge
