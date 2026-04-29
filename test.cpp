// #include "inc/port.h"
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include <cstdint>
#include <optional>

#include "internal/logging.hpp"

#include "bridge/bridge.hpp"
#include "callback.hpp"

#include <condition_variable>
#include <mutex>

#include <libusb.h>

std::condition_variable c_v;
std::mutex mut;
bool cont{false};

void cb(result_t r) {
  LOGD("Got result");

  if (r.type == result_type::string) { LOGD("Response: {}", r.data.string); }

  cont = true;
  c_v.notify_one();
}

int test_bridge() {
  auto handle{bridge_create()};
  bridge_init(handle);
  bridge_register_cb(handle, cb);

  enum class step {
    open,
    config,
    claim,
    done,
  } s{step::open};

  // Open and configure
  while (s != step::done) {
    int rc{0};
    switch (s) {
      case step::open:
        rc = bridge_open(handle);
        s = step::config;
        break;
      case step::config:
        rc = bridge_config(handle);
        s = step::claim;
        break;
      case step::claim:
        rc = bridge_claim(handle);
        s = step::done;
        break;
      default: break;
    }
    if (rc != LIBUSB_SUCCESS) {
      bridge_destroy(handle);
      libusb_exit(nullptr);
      abort();
    }
  }

  // ping
  char buffer[64u];
  std::unique_lock<std::mutex> lock(mut);
  if (!bridge_com_ping(handle)) abort();
  // c_v.wait(lock, [] { return cont; });

  auto r = bridge_result(handle);
  if (r.type == result_type::string) LOGD("Result: {}", r.data.string);

  bridge_com_mdu_ein(handle);
  bridge_result(handle);
  bridge_mdu_ein_enter_dcc_zpp(handle);
  bridge_result(handle);
  bridge_mdu_ein_cv_read(handle, 7u);

  r = bridge_result(handle);
  if (r.type == result_type::cv) {
    LOGD("Cv 7 is {} ", r.data.value);
  } else LOGE("FUGG");

  bridge_com_reset(handle);
  bridge_result(handle);

  bridge_release(handle);
  bridge_close(handle);
  bridge_destroy(handle);

  libusb_exit(nullptr);
  return 0;
}

int main(int argc, char** argv) { return test_bridge(); }
// int main(int argc, char** argv) { return test(); }