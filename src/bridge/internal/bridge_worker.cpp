#include "bridge/internal/bridge_worker.hpp"
#include <thread>

namespace bridge {

Worker::Worker(Context& ctx) : _ctx{ctx}, exit{false} {
  _thread = std::thread(&Worker::loop, this);
}
Worker::~Worker() {
  {
    std::lock_guard<std::mutex> lock(_mut_t);
    exit = true;
  }
  _cv.notify_one();
  if (_thread.joinable()) _thread.join();
}

int Worker::loop() {
  // Loop till death
  while (true) {
    {
      std::unique_lock<std::mutex> lock(_mut_t);
      _cv.wait(lock, [this] { return _t != nullptr || exit; });
    }
    if (exit) break;

    _t->execute();
    _t->push();
    _t.reset();
  }
  return 0;
}

}  // namespace bridge