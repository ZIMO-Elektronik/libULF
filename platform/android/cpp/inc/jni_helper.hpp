/**
 * Copyright (C) 2026 ZIMO Elektronik
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
 * JNI Helpers
 *
 * \file    platform/android/cpp/inc/jni_helper.hpp
 * \author  Jonas Gahlert
 * \date    04.09.2026
 */

#include <jni.h>
#include <ulf/c/libulf.h>
#include <cassert>

/**
 * Throws a LibULFException
 *
 * \warning
 * If for some reason no such class exists, the app will crash
 *
 * \param env JNI Environment
 * \param err Occurred error
 */
void throwLibULFException(JNIEnv* env, libulf_error err) {
  jclass exceptionClass = env->FindClass("com/zimo/ulf/LibULFException");

  if (exceptionClass == nullptr) {
    assert(false);
    return;
  } // Class not found = Bad..

  jmethodID constructor = env->GetMethodID(exceptionClass, "<init>", "(I)V");

  if (constructor == nullptr) {
    assert(false);
    return;
  }

  jobject exceptionObject =
    env->NewObject(exceptionClass, constructor, static_cast<jint>(err));

  if (exceptionObject != nullptr) {
    env->Throw(static_cast<jthrowable>(exceptionObject));
  } else {
    assert(false);
  }
}

/**
 * Helper for functions returning boolean
 *
 * \details
 * Returns the result, while wrapping a possible error into an exception.
 *
 * \tparam F    Function type
 * \tparam Args Function Arguments type list
 * \param env   JNI Environment
 * \param f     Function
 * \param args  Function Arguments
 *
 * \return jboolean Result of Function
 */
template<typename F, typename... Args>
jboolean boolFn(JNIEnv* env, F&& f, Args&&... args) {
  bool r{false};

  auto const err{f(std::forward<Args>(args)..., &r)};

  if (err != LIBULF_OK) {
    throwLibULFException(env, err);
    return JNI_FALSE;
  }

  return r == true ? JNI_TRUE : JNI_FALSE;
}

/**
 * Helper for functions returning a string
 *
 * \details
 * Returns the result, while wrapping a possible error into an exception.
 *
 * \tparam F    Function type
 * \tparam Args Function Arguments type list
 * \param env   JNI Environment
 * \param f     Function
 * \param args  Function Arguments
 *
 * \return jstring Result of Function
 */
template<typename F, typename... Args>
jstring stringFn(JNIEnv* env, F&& f, Args&&... args) {
  constexpr size_t bufferCapacity = 128uz;

  // A byte for the null terminator (if its missing later)
  std::array<char, bufferCapacity + 1uz> buffer{};

  size_t length = bufferCapacity;

  libulf_error const err =
    std::forward<F>(f)(std::forward<Args>(args)..., buffer.data(), &length);

  if (err != LIBULF_OK) {
    throwLibULFException(env, err);
    return nullptr;
  }

  // Should not be needed, but better save than sorry
  length = std::min(length, bufferCapacity);

  // Add null terminator for conversion to utf string
  buffer[length] = '\0';

  return env->NewStringUTF(buffer.data());
}

/**
 * Helper for functions returning byte (uint8_t)
 *
 * \details
 * Returns the result, while wrapping a possible error into an exception.
 *
 * \tparam F    Function type
 * \tparam Args Function Arguments type list
 * \param env   JNI Environment
 * \param f     Function
 * \param args  Function Arguments
 *
 * \return jint Result of Function
 */
template<typename F, typename... Args>
jint valueFn(JNIEnv* env, F&& f, Args&&... args) {
  int r{};

  auto const err{f(std::forward<Args>(args)..., &r)};

  if (err != LIBULF_OK) {
    throwLibULFException(env, err);
    return JNI_FALSE;
  }

  return r;
}

/** --------------------------------------------------
 *  Cast Helpers
 *  --------------------------------------------------
 */

/// Cast to libulf
constexpr auto to_lib(jlong handle) {
  return reinterpret_cast<libulf_handle>(handle);
}

/// Cast to zpp
constexpr auto to_zpp(jlong handle) {
  return reinterpret_cast<zpp_handle>(handle);
}

/// Cast to zsu
constexpr auto to_zsu(jlong handle) {
  return reinterpret_cast<zsu_handle>(handle);
}

/// Cast to bool
constexpr auto to_bool(jint i) { return static_cast<bool>(i); }

/// Cast from libulf
constexpr jlong to_jlong(libulf_handle handle) {
  return reinterpret_cast<jlong>(handle);
}

/// Cast from zpp
constexpr jlong to_jlong(zpp_handle handle) {
  return reinterpret_cast<jlong>(handle);
}

/// Cast from zsu
constexpr jlong to_jlong(zsu_handle handle) {
  return reinterpret_cast<jlong>(handle);
}

/// Cast error
constexpr jint to_jint(libulf_error e) { return static_cast<jint>(e); }

/// Cast bool
constexpr jint to_jint(bool b) { return static_cast<jint>(b); }
