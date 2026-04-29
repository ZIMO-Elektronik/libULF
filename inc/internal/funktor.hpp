#pragma once

#include "callback.hpp"

namespace internal {

struct Funktor {
  Funktor(bridge_callback cb);
  virtual ~Funktor();

  void operator()(result_t r);

private:
  bridge_callback _cb;
};

}  // namespace internal