/**
 * JNI Defines
 *
 * \file    inc/libklug/internal/platform/android/jni_defines.hpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include <jni.h>

/// Defines the JNI Java class this api should bind to
#ifndef JNI_CLASS_PATH
#  define JNI_CLASS_PATH Java_at_zimo_klug_KLUGAdapter_
#endif

#define JNI_CONCAT2(a, b) a##b
#define JNI_CONCAT(a, b) JNI_CONCAT2(a, b)

/// Defines a JNI method to reduce definition boilerplate
#define JNI_METHOD(return_type, name, ...)                                     \
  JNIEXPORT return_type JNICALL JNI_CONCAT(JNI_CLASS_PATH, name)(              \
    [[maybe_unused]] JNIEnv * env,                                             \
    [[maybe_unused]] jclass clazz,                                             \
    ##__VA_ARGS__)
