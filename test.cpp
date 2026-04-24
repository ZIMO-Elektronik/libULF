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
}

int test_bridge() {}

int main(int argc, char** argv) {
  return test(); 
}