#pragma once

#include <jni.h>
#include <thread>
#include "callback.hpp"

namespace internal {

/**
 * \brief Android Funktor
 *
 */
struct AndroidFunktor {
  AndroidFunktor();
  ~AndroidFunktor();

  void operator()(result_t r);

  void loop();

private:
  bool attach();
  bool detach();

  JavaVM* _vm;     ///< JVM reference
  jobject _cbRef;  ///< Callback reference

  std::thread _thread;          ///< JVM attached thread
  std::condition_variable _cv;  ///< Wait condition

  std::unique_ptr<result_t> _result;  ///< Result
  std::mutex _mut_result;             ///< Result mutex
};

using Funktor = AndroidFunktor;

}  // namespace internal