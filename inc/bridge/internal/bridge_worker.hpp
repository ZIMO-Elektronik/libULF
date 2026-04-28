#pragma once

#include <condition_variable>
#include <thread>
#include "bridge_context.hpp"
#include "internal/transmission.hpp"

namespace bridge {

struct Worker {
  Worker(Context& ctx);
  ~Worker();

  template<typename T, typename... Args>
  requires std::derived_from<T, transmission::Transmission> &&
           std::constructible_from<T, Args...>
  bool emplace(Args&&... args) {
    std::unique_lock<std::mutex> lock(_mut_t);

    // Check for existing transmission
    if (_t) { return false; }

    // Prepare transmission and result
    _t = std::make_unique<T>(std::forward<Args>(args)...);
    std::construct_at(&_promise);
    std::construct_at(&_ctx.result, _promise.get_future());

    // Notify worker thread
    lock.unlock();
    _cv.notify_one();
    return true;
  }

  // Thread loop
  void loop();

private:
  Context& _ctx;  ///< Bridge context

  std::thread _thread;                             ///< Thread
  std::promise<result_t> _promise;                 ///< Promise of result
  std::mutex _mut_t;                               ///< Transmission mutex
  std::condition_variable _cv;                     ///< Wait condition
  std::unique_ptr<transmission::Transmission> _t;  ///< Current transmission

  bool exit;
};

}  // namespace bridge