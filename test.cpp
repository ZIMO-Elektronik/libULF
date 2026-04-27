// #include "inc/port.h"
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include <cstdint>
#include <optional>

#include "com.hpp"
#include "internal/logging.hpp"
#include "new_port.hpp"
#include "susiv2.hpp"

#include "bridge/bridge.hpp"

int test() {
  init();
  if (!open_klug()) {
    libusb_exit(nullptr);
    return -1;
  }
  // ping_klug();
  // close_klug();

  char buffer[64u];
  if (com_ping(buffer, 64u) != 0) abort();

  std::string ping{buffer};

  LOGD("Result: {}", ping);

  bool result{};

  if (com_susiv2(&result) != 0) abort();
  if (result) {
    LOGD("Entered SUSIV2 Mode");
  } else {
    LOGE("Unable to enter SUSIV2 Mode");
    abort();
  }

  if (susiv2_features(&result) != 0) {
    LOGE("Unable to request Features");
    com_reset(&result);
    abort();
  }

  uint8_t value{};
  if (susiv2_cv_read(8u, &value) != 0) {
    LOGE("Could not read Cv");
    com_reset(&result);
    abort();
  }

  LOGD("Read Cv 8 = {}", static_cast<int>(value));
  com_reset(&result);

  close_klug();
  return 0;
}

int test_bridge() {
  auto handle{bridge_create()};
  bridge_init(handle);

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
        rc = bridge_configure(handle);
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
  if (bridge_com_ping(handle, buffer, 64u) != 0) abort();

  std::string ping{buffer};

  LOGD("Result: {}", ping);

  bridge_release(handle);
  bridge_close(handle);
  bridge_destroy(handle);

  libusb_exit(nullptr);
  return 0;
}

int main(int argc, char** argv) { return test_bridge(); }
// int main(int argc, char** argv) { return test(); }