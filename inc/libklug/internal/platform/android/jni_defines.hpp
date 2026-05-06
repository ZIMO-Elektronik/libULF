/**
 * JNI Defines
 *
 * \file    inc/libklug/internal/platform/android/jni_defines.hpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include <jni.h>

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
