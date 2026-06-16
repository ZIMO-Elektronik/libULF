/**
 * Internal bridge worker
 *
 * \file    src/libklug/internal/bridge/bridge_worker.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_worker.hpp"
#include <future>
#include <thread>
#include "libklug/internal/exception/e_generic.hpp"
#include "libklug/internal/exception/e_libusb.hpp"
#include "libklug/internal/logging.hpp"

namespace bridge {

/**
 * CTor
 *
 * \details Starts thread
 *
 * \param ctx Context
 */
Worker::Worker(Context& ctx) : _ctx{ctx}, exit{false} {
  _thread = std::thread(&Worker::loop, this);
}

/**
 * DTor
 *
 * \details Joins thread
 *
 * \warning Because of the thread context, DTor must be called from constructing
 * thread
 *
 */
Worker::~Worker() {
  {
    std::lock_guard<std::mutex> lock(_mut_t);
    exit = true;
  }
  _cv.notify_one();
  if (_thread.joinable()) _thread.join();
}

/**
 * Thread loop
 *
 * \details Will execute current transmission and update cb on completion
 *
 */
void Worker::loop() {
  // Loop till death
  while (true) {
    {
      std::unique_lock<std::mutex> lock(_mut_t);
      _cv.wait(lock, [this] { return _t != nullptr || exit; });
    }
    if (exit) break;

    try {
      _t->execute();
      auto r{_t->evaluate()};
      _t.reset();
      _promise.set_value(r);
      if (_ctx.cb) (*_ctx.cb)(r);
    } catch (except::generic_error e) {
      _t.reset();
      LOGE("{}", e.what());
      _promise.set_exception(std::current_exception());
      if (_ctx.cb) (*_ctx.cb)(static_cast<res::Error>(e));
    } catch (except::libusb_error e) {
      _t.reset();
      LOGE("{}", e.what());
      _promise.set_exception(std::current_exception());
      if (_ctx.cb) (*_ctx.cb)(static_cast<res::LibusbError>(e));
    } catch (std::exception e) {
      _t.reset();
      LOGE("{}", e.what());
      _promise.set_exception(std::current_exception());
      if (_ctx.cb) (*_ctx.cb)(err::Error::unknown);
    }
  }
}

} // namespace bridge
