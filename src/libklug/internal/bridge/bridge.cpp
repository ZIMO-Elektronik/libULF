/**
 * Internal Bridge config
 *
 * \file    src/libklug/internal/bridge/bridge.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge.hpp"
#include "libklug/internal/exception/e_generic.hpp"

namespace bridge {

Bridge::Bridge(std::shared_ptr<internal::IConnection> conn) : _ctx{conn} {}

/**
 * Init USB backend
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 *
 */
void Bridge::init() { return _ctx.connection->init(); }

/**
 * Open usb device
 *
 * \param vid   VID
 * \param pid   PID
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
void Bridge::open(uint16_t vid, uint16_t pid) {
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
void Bridge::openFd(int Fd) { return _ctx.connection->openFd(Fd); }

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
