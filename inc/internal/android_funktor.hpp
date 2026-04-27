#pragma once

#include <jni.h>
#include "callback.hpp"

namespace internal {

struct AndroidFunktor {
  AndroidFunktor();
  ~AndroidFunktor();

  void operator()(result_t);

private:
  /// JNI
  JavaVM* _vm;
  jobject _cbRef;
};

using Funktor = AndroidFunktor;

}  // namespace internal