/**
 * JNI context for Android
 *
 * \file    inc/libklug/internal/platform/android/jni_context.hpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include <jni.h>

namespace internal {

/**
 * JNI Context
 *
 * \details
 * This keeps a pointer to the JVM for later creation of JNIEnv instances. This
 * can be used to push async operations to the Java side safely.
 *
 * An example would be a callback called from a cpp background thread.
 *
 */
struct JNIContext {
  JNIContext() = default;
  JNIContext(JavaVM* vm_);
  ~JNIContext();

  JavaVM* vm{};
};

inline JNIContext jni_ctx{};

} // namespace internal
