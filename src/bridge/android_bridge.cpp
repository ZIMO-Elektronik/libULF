#include <jni.h>
#include <array>

#include "bridge/bridge.hpp"

#ifndef JNI_CLASS_PATH
static_assert(false, "Must name a JNI class");
#endif

#ifndef JNI_RESULT_PATH
static_assert(false, "Must name a Result class");
#endif

#define JNI_METHOD_PREFIX Java_com_example_test_1libklug_NativeLib_

#define JNI_CONCAT2(a, b) a##b
#define JNI_CONCAT(a, b) JNI_CONCAT2(a, b)

#define JNI_METHOD(return_type, name, ...)                                     \
  JNIEXPORT return_type JNICALL JNI_CONCAT(JNI_CLASS_PATH, name)(              \
    [[maybe_unused]] JNIEnv * env,                                             \
    [[maybe_unused]] jobject thiz,                                             \
    ##__VA_ARGS__)

extern "C" {

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

JNI_METHOD(jlong, bridge_1create) { return (jlong)bridge_create(); }

JNI_METHOD(void, bridge_1destroy, jlong handle) {
  return bridge_destroy(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(void, bridge_1register_1cb, jlong handle, jobject cb) {
  return;  // bridge_register_cb(cb);
}

JNI_METHOD(jobject, bridge_1result, jlong handle) {
  auto const r{bridge_result(reinterpret_cast<bridge_handle>(handle))};

  switch (r.type) {
    case result_type::string: {
      jclass c = env->FindClass(JNI_RESULT_PATH "$String");
      jmethodID init = env->GetMethodID(c, "<init>", "(Ljava/lang/String;)V");
      return env->NewObject(c, init, env->NewStringUTF(r.data.string));
    }
    case result_type::cv: {
      jclass c = env->FindClass(JNI_RESULT_PATH "$Cv");
      jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
      return env->NewObject(c, init, r.data.value);
    }
    case result_type::status: {
      jclass c = env->FindClass(JNI_RESULT_PATH "$Status");
      jmethodID init = env->GetMethodID(c, "<init>", "(I)V");
      return env->NewObject(c, init, r.data.success);
    }
    default: return nullptr;
  }
}

JNI_METHOD(jint, bridge_1init, jlong handle) {
  return bridge_init(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(
  jint, bridge_1open, jlong handle, jint vid = 0x1FC9u, jint pid = 0x81C1u) {
  return bridge_open(reinterpret_cast<bridge_handle>(handle), vid, pid);
}

JNI_METHOD(jint, bridge_1openFd, jlong handle, jint Fd) {
  return bridge_openFd(reinterpret_cast<bridge_handle>(handle), Fd);
}

JNI_METHOD(jint, bridge_1config, jlong handle) {
  return bridge_config(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1claim, jlong handle) {
  return bridge_claim(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1release, jlong handle) {
  return bridge_release(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(void, bridge_1close, jlong handle) {
  return bridge_close(reinterpret_cast<bridge_handle>(handle));
}

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1com_1ping, jlong handle) {
  return bridge_com_ping(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1com_1reset, jlong handle) {
  return bridge_com_reset(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1com_1susiv2, jlong handle) {
  return bridge_com_susiv2(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1com_1mdu_1ein, jlong handle) {
  return bridge_com_mdu_ein(reinterpret_cast<bridge_handle>(handle));
}

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1susiv2_1cv_1read, jlong handle, jint cv) {
  return bridge_susiv2_cv_read(reinterpret_cast<bridge_handle>(handle), cv);
}

JNI_METHOD(jint, bridge_1susiv2_1features, jlong handle) {
  return bridge_susiv2_features(reinterpret_cast<bridge_handle>(handle));
}

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1mdu, jlong handle) {
  return bridge_mdu_ein_enter_mdu(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1dcc_1zsu, jlong handle) {
  return bridge_mdu_ein_enter_dcc_zsu(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1dcc_1zpp, jlong handle) {
  return bridge_mdu_ein_enter_dcc_zpp(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1mdu_1ein_1cv_1read, jlong handle, jint cv) {
  return bridge_mdu_ein_cv_read(reinterpret_cast<bridge_handle>(handle), cv);
}

JNI_METHOD(jint, bridge_1mdu_1ein_1ping, jlong handle) {
  return bridge_mdu_ein_ping(reinterpret_cast<bridge_handle>(handle));
}
}