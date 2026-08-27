#include <jni.h>
#include <libklug.h>
#include <cassert>

/**
 * Throws a LibKLUGException
 *
 * \warning
 * If for some reason no such class exists, the app will crash
 *
 * \param env JNI Environment
 * \param err Occurred error
 */
void throwLibKLUGException(JNIEnv* env, libklug_error err) {
  jclass exceptionClass = env->FindClass("com/zimo/klug/LibKLUGException");

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
  libklug_bool r{LIBKLUG_FALSE};

  auto const err{f(std::forward<Args>(args)..., &r)};

  if (err != LIBKLUG_OK) {
    throwLibKLUGException(env, err);
    return JNI_FALSE;
  }

  return r == LIBKLUG_TRUE ? JNI_TRUE : JNI_FALSE;
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

  libklug_error const err =
    std::forward<F>(f)(std::forward<Args>(args)..., buffer.data(), &length);

  if (err != LIBKLUG_OK) {
    throwLibKLUGException(env, err);
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
jint byteFn(JNIEnv* env, F&& f, Args&&... args) {
  uint8_t r{};

  auto const err{f(std::forward<Args>(args)..., &r)};

  if (err != LIBKLUG_OK) {
    throwLibKLUGException(env, err);
    return JNI_FALSE;
  }

  return r;
}

/** --------------------------------------------------
 *  Cast Helpers
 *  --------------------------------------------------
 */

/// Cast to libklug
constexpr auto to_lib(jlong handle) {
  return reinterpret_cast<libklug_handle>(handle);
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
constexpr auto to_bool(jint i) { return static_cast<libklug_bool>(i); }

/// Cast from libklug
constexpr jlong to_jlong(libklug_handle handle) {
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
constexpr jint to_jint(libklug_error e) { return static_cast<jint>(e); }

/// Cast bool
constexpr jint to_jint(libklug_bool b) { return static_cast<jint>(b); }
