/**
 * Copyright (C) 2026 [ZIMO Elektronik]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 *
 *
 *
 *
 *
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
