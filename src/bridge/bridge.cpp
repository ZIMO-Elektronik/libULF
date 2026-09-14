/**
 * Copyright (C) 2026 [ZIMO Elektronik]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 *
 *
 *
 *
 *
 * Internal Bridge config
 *
 * \file    src/bridge/bridge.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge.hpp"

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
 *
 * \throws ulf_error   First error occurred
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
 *
 * \throws ulf_error   First error occurred
 */
void Bridge::openFd(int Fd) { return _ctx.connection->openFd(Fd); }

/**
 * Close usb device
 *
 * \throws ulf_error   First error occurred
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
