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

    _t = std::make_unique<T>(std::forward<Args>(args)...);

    // Notify worker thread
    lock.unlock();
    _cv.notify_one();
    return true;
  }

  // Thread loop
  int loop();

private:
  Context& _ctx;

  std::thread _thread;
  std::mutex _mut_t;
  std::condition_variable _cv;
  std::unique_ptr<transmission::Transmission> _t;

  bool exit;
};

}  // namespace bridge