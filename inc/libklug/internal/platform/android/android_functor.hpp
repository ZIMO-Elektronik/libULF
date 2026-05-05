/**
 * Android Functor
 *
 * \file    inc/libklug/internal/platform/android/android_functor.hpp
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include <jni.h>
#include <optional>
#include <thread>
#include "libklug/internal/i_functor.hpp"

namespace internal {

/**
 * AndroidFunctor
 *
 * \note Funktor for android, since a thread calling a callback via JNI needs to
 * be attached to the JVM
 *
 */
struct AndroidFunctor : public IFunktor {
  AndroidFunctor(JNIEnv* env, jobject instannce, jobject cb);
  virtual ~AndroidFunctor();

  virtual void operator()(result_t const& r) override;

  void loop();

private:
  bool attach();
  void detach();
  void call(result_t const& r);

  JNIEnv* env();

  JavaVM* _vm;    ///< JVM reference
  jobject _cbRef; ///< Callback reference
  jmethodID _mid; ///< Callback method id

  std::thread _thread;         ///< JVM attached thread
  std::condition_variable _cv; ///< Wait condition
  bool _exit{false};           ///< Exit condition

  std::optional<result_t> _r{}; ///< Result
  std::mutex _mut_r{};          ///< Result mutex
};

} // namespace internal
