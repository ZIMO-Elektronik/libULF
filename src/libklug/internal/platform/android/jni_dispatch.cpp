#include "libklug/internal/platform/android/jni_dispatch.hpp"
#include "libklug/internal/logging.hpp"
#include "libklug/internal/platform/android/jni_context.hpp"
#include "libklug/result/dispatch.hpp"
#include "libklug/result/result.hpp"

namespace res {

jobject jni_dispatch(JNIEnv* env, Result const& r) {
  return std::visit(
    overloads{[&](String s) {
                LOGD("Returning String: {}", static_cast<std::string>(s));
                auto c{internal::jni_ctx.nativeResult_string};
                jmethodID init =
                  env->GetMethodID(c, "<init>", "(Ljava/lang/String;)V");
                return env->NewObject(c, init, env->NewStringUTF(s->c_str()));
              },
              [&](Status s) {
                auto c{internal::jni_ctx.nativeResult_status};
                jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
                return env->NewObject(c, init, s);
              },
              [&](Cv cv) {
                auto c{internal::jni_ctx.nativeResult_cv};
                jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
                return env->NewObject(c, init, cv);
              },
              [&](Error e) {
                auto c{internal::jni_ctx.nativeResult_error};
                jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
                return env->NewObject(c, init, e);
              },
              [&](LibusbError e) {
                auto c{internal::jni_ctx.nativeResult_usbError};
                jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
                return env->NewObject(c, init, e);
              },
              [&]() { return nullptr; }},
    r);
}

} // namespace res
