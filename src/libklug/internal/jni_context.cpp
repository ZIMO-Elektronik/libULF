/**
 * JNI Context
 *
 * \file    src/internal/jni_context.cpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#include "internal/jni_context.hpp"
#include "bridge/jni_defines.hpp"
#include "internal/logging.hpp"

namespace internal {

JNIContext::JNIContext(JavaVM* vm_) : vm{vm_} {
  LOGD("Creating JNI context");

  JNIEnv* env;
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return;

  LOGD("Finding JNI classes");

  auto c{env->FindClass(JNI_RESULT_PATH "$String")};
  nativeResult_string = static_cast<jclass>(env->NewGlobalRef(c));

  LOGD("Finding Status class");

  c = env->FindClass(JNI_RESULT_PATH "$Status");
  nativeResult_status = static_cast<jclass>(env->NewGlobalRef(c));

  c = env->FindClass(JNI_RESULT_PATH "$Cv");
  nativeResult_cv = static_cast<jclass>(env->NewGlobalRef(c));

  c = env->FindClass(JNI_RESULT_PATH "$Error");
  nativeResult_error = static_cast<jclass>(env->NewGlobalRef(c));

  c = env->FindClass(JNI_RESULT_PATH "$UsbError");
  nativeResult_usbError = static_cast<jclass>(env->NewGlobalRef(c));
}

JNIContext::~JNIContext() {
  JNIEnv* env;
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return;

  if (nativeResult_string) env->DeleteGlobalRef(nativeResult_string);
  if (nativeResult_status) env->DeleteGlobalRef(nativeResult_status);
  if (nativeResult_cv) env->DeleteGlobalRef(nativeResult_cv);
  if (nativeResult_error) env->DeleteGlobalRef(nativeResult_error);
  if (nativeResult_usbError) env->DeleteGlobalRef(nativeResult_usbError);
}

} // namespace internal
