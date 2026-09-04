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
 * Internal bridge config
 *
 * \file    src/bridge/bridge.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "bridge_com.hpp"
#include "bridge_context.hpp"
#include "bridge_mdu_ein.hpp"
#include "bridge_susiv2.hpp"
#include "bridge_zpp.hpp"
#include "bridge_zsu.hpp"

namespace bridge {

/**
 * Bridge composit class
 *
 * \details Combosit class consisting of the protocol bridges. Used to keep
 * transfers in a controllable context
 *
 * \details This class is an attempt to provide all necessary ULF_COM update and
 * soundload protocols behind a singular interface.
 *
 * \note The bridge needs to be fully connected and configured to be usable. Two
 * open bridges result in UB, since the ULF_COM transmissions are synchronous by
 * design.
 *
 * \note To use, either \ref Bridge::open a device (or \ref Bridge::openFd on
 * e.g. Android). Afterwards, \ref Bridge::config and \ref Bridge::claim
 *
 * \warning It is imperative, that the bridge is fully released before the
 * object is deleted. Use \ref Bridge::release and \ref Bridge::close
 *
 * \todo Maybe the process of opening, connecting and closing could be provided
 * with AIO methods like `connect` and `disconnect`
 */
struct Bridge {
  Bridge() = default;
  Bridge(std::shared_ptr<internal::IConnection> conn);

  void init();

  void open(uint16_t vid = 0x1FC9u, uint16_t pid = 0x81C1u);
  void openFd(int Fd);

  void close();

  COM& com();
  SUSIV2& susiv2();
  MDU_EIN& mdu_ein();

  ZPP& zpp();
  ZSU& zsu();

  void lastWhat(char const* str);
  char const* lastWhat() const;

private:
  Context _ctx; ///< Bridge context
  ZPP _zpp{};   ///< ZPP bridge
  ZSU _zsu{};   ///< ZSU bridge

  COM _com{_ctx};                     ///< COM bridge
  SUSIV2 _susiv2{_ctx, _zpp};         ///< SUSIV2 bridge
  MDU_EIN _mdu_ein{_ctx, _zpp, _zsu}; ///< MDU_EIN bridge

  char const* _lastWhat{}; ///< Last exception string
};

} // namespace bridge
