/**
 * JNI Context
 *
 * \file    src/libklug/internal/platform/android/jni_context.cpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#include "inc/jni_context.hpp"
#include "inc/jni_defines.hpp"

namespace internal {

JNIContext::JNIContext(JavaVM* vm_) : vm{vm_} {
  JNIEnv* env;
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return;
}

JNIContext::~JNIContext() {
  JNIEnv* env;
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return;
}

} // namespace internal
