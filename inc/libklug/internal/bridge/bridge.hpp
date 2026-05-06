/**
 * Internal bridge config
 *
 * \file    inc/bridge/internal/bridge.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "bridge_com.hpp"
#include "bridge_context.hpp"
#include "bridge_mdu_ein.hpp"
#include "bridge_susiv2.hpp"
#include "bridge_worker.hpp"
#include "bridge_zpp.hpp"
#include "bridge_zsu.hpp"
#include "libklug/callback/callback.h"
#include "libklug/callback/i_functor.hpp"

namespace bridge {

/**
 * Bridge composit class
 *
 * \details Combosit class consisting of the protocol bridges. Used to keep
 * transfers in a controllable context
 *
 * \details This class is an attempt to provide all necessary ULF_COM update and
 * soundload protocols behind a singular interface. As a concept, all
 * transmissions run asynchronous via the \ref Bridge::Worker object. The result
 * can either be waited upon using \ref Bridge::result, or get it delivered when
 * registering a callback with \ref Bridge::registerCB.
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
  int init();

  void registerCB(std::unique_ptr<internal::IFunctor> cb);
  void deregisterCB();
  res::Result result();

  int open(uint16_t vid = 0x1FC9u, uint16_t pid = 0x81C1u);
  int openFd(int Fd);

  int config();
  int claim();

  int release();
  void close();

  COM& com();
  SUSIV2& susiv2();
  MDU_EIN& mdu_ein();

  ZPP& zpp();

private:
  Context _ctx; ///< Bridge context
  ZPP _zpp{};   ///< ZPP bridge

  Worker _worker{_ctx}; ///< Worker

  COM _com{_ctx, _worker};             ///< COM bridge
  SUSIV2 _susiv2{_ctx, _worker, _zpp}; ///< SUSIV2 bridge
  MDU_EIN _mdu_ein{_ctx, _worker};     ///< MDU_EIN bridge
};

} // namespace bridge
