/**
 * Internal bridge worker
 *
 * \file    inc/libklug/internal/bridge/bridge_worker.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <condition_variable>
#include <thread>
#include "bridge_context.hpp"
#include "libklug/internal/transmission/transmission_base.hpp"
#include "libklug/result/result.hpp"

namespace bridge {

/**
 * Bridge worker
 *
 * \details Used to make transmissions async.
 *
 * \note Thread is created in CTor and joined in DTor without user intervention
 *
 * \note Only ONE transmission can be running at a time.
 *
 * \todo Currently, close to no error handling is present.
 *
 */
struct Worker {
  Worker(Context& ctx);
  ~Worker();

  /**
   * Emplace a transmission
   *
   * \tparam T      Type of transmission
   * \tparam Args   Arg list for CTor
   *
   * \param args    Transmission CTor args
   * \return bool
   * \retval true   Transmission emplaced
   * \retval false  Busy with other transmission
   */
  template<typename T, typename... Args>
  requires std::derived_from<T, transmission::ITransmission> &&
           std::constructible_from<T, Args...>
  bool emplace(Args&&... args) {
    std::unique_lock<std::mutex> lock(_mut_t);

    // Check for existing transmission
    if (_t) { return false; }

    // Prepare transmission and result
    _t = std::make_unique<T>(std::forward<Args>(args)...);
    _promise = std::promise<res::Result&>();
    _ctx.result = _promise.get_future();

    // Notify worker thread
    lock.unlock();
    _cv.notify_one();
    return true;
  }

  // Thread loop
  void loop();

private:
  Context& _ctx; ///< Bridge context

  std::thread _thread;                             ///< Thread
  std::promise<res::Result&> _promise;             ///< Promise of result
  std::mutex _mut_t;                               ///< Transmission mutex
  std::condition_variable _cv;                     ///< Wait condition
  std::unique_ptr<transmission::ITransmission> _t; ///< Current transmission
  std::unique_ptr<res::Result> _r;                 ///< Last result

  bool exit;
};

} // namespace bridge
