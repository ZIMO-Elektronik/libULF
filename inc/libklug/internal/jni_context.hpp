/**
 * JNI context for Android
 *
 * \file    inc/internal/jni_context.hpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include <jni.h>

namespace internal {

struct JNIContext {
  JNIContext() = default;
  JNIContext(JavaVM* vm_);
  ~JNIContext();

  JavaVM* vm{};

  // Result classes
  jclass nativeResult_string{};
  jclass nativeResult_status{};
  jclass nativeResult_cv{};
  jclass nativeResult_error{};
  jclass nativeResult_usbError{};
};

inline JNIContext jni_ctx{};

} // namespace internal
