#include "internal/android_funktor.hpp"

namespace internal {

Android::Funktor::AndroidFunktor() {
  // Create thread
}

AndroidFunktor::~AndroidFunktor() {
  // Join thread
}

void AndroidFunktor::loop() {
  // Attach thread

  while (true) {
    // Push result to JVM
  }

  // Detach thread
}

bool AndroidFunktor::attach() {
  // Attach thread to JVM
}

bool AndroidFunktor::detach() {
  // Detach thread from JVM
}

}  // namespace internal