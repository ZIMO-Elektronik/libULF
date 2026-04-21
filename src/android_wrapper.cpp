#include <jni.h>

#include "new_port.hpp"

extern "C" {

JNIEXPORT jint JNICALL
Java_com_example_test_1libklug_NativeLib_init(JNIEnv *env, jobject thiz) {
  return init();
}

JNIEXPORT jint JNICALL
Java_com_example_test_1libklug_NativeLib_open_1klug(JNIEnv *env, jobject thiz) {
  return open_klug();
}

JNIEXPORT jint JNICALL Java_com_example_test_1libklug_NativeLib_openWithFd(
    JNIEnv *env, jobject thiz, jint fd) {
  return openWithFd(fd);
}

JNIEXPORT jstring JNICALL
Java_com_example_test_1libklug_NativeLib_ping_1klug(JNIEnv *env, jobject thiz) {
  return env->NewStringUTF(ping_klug());
}

JNIEXPORT jint JNICALL Java_com_example_test_1libklug_NativeLib_close_1klug(
    JNIEnv *env, jobject thiz) {
  return close_klug();
}
}