#include <jni.h>
#include <array>

#include "bridge/bridge.hpp"

#define JNI_METHOD_PREFIX Java_com_example_test_1libklug_NativeLib_

#define JNI_CONCAT2(a, b) a##b
#define JNI_CONCAT(a, b) JNI_CONCAT2(a, b)

#define JNI_METHOD(return_type, name, ...)                                     \
  JNIEXPORT return_type JNICALL JNI_CONCAT(JNI_METHOD_PREFIX, name)(           \
    [[maybe_unused]] JNIEnv * env,                                             \
    [[maybe_unused]] jobject thiz,                                             \
    ##__VA_ARGS__)

extern "C" {

JNI_METHOD(jlong, bridge_1create) { return (jlong)bridge_create(); }

JNI_METHOD(void, bridge_1destroy, jlong handle) {
  return bridge_destroy(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(void, bridge_1register_1cb, jlong handle, jobject cb) {
  return;  // bridge_register_cb(cb);
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

JNI_METHOD(jstring, bridge_1com_1ping, jlong handle) {
  std::array<char, 64> buffer;

  bridge_com_ping(
    reinterpret_cast<bridge_handle>(handle), buffer.data(), buffer.size());

  return env->NewStringUTF(buffer.data());
}
}