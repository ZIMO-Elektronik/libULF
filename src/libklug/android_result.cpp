#include "libklug/internal/platform/android/android_result.hpp"
#include "libklug/internal/logging.hpp"
#include "libklug/internal/platform/android/jni_context.hpp"

jobject dispatch(JNIEnv* env, result_t const& r) {
  switch (r.type) {
    case result_type::string: {
      auto c{internal::jni_ctx.nativeResult_string};
      jmethodID init = env->GetMethodID(c, "<init>", "(Ljava/lang/String;)V");
      return env->NewObject(c, init, env->NewStringUTF(r.data.string));
    }
    case result_type::cv: {
      auto c{internal::jni_ctx.nativeResult_cv};
      jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
      return env->NewObject(c, init, r.data.value);
    }
    case result_type::status: {
      auto c{internal::jni_ctx.nativeResult_status};
      jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
      return env->NewObject(c, init, r.data.success);
    }
    case result_type::error: {
      auto c{internal::jni_ctx.nativeResult_error};
      jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
      return env->NewObject(c, init, r.data.error);
    }
    case result_type::libusb_error: {
      auto c{internal::jni_ctx.nativeResult_usbError};
      jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
      return env->NewObject(c, init, r.data.libusb_error);
    }
    default: return nullptr;
  }
}
