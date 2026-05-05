/**
 * Internal bridge worker
 *
 * \file    src/brigdge/internal/bridge_worker.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge/internal/bridge_worker.hpp"
#include <future>
#include <thread>

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

    _t->execute();
    auto r{_t->evaluate()};
    _promise.set_value(r);

    // Push result to cb
    if (_ctx.cb) (*_ctx.cb)(r);

    _t.reset();
  }
}

} // namespace bridge
