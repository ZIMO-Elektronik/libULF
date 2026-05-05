#include "internal/android_funktor.hpp"

#include <cassert>
#include <mutex>
#include "android_result.hpp"
#include "bridge/jni_defines.hpp"
#include "internal/logging.hpp"

namespace internal {

AndroidFunktor::AndroidFunktor(JNIEnv* env, jobject instance, jobject cb) {
  // Get JNI environment
  auto rc{env->GetJavaVM(&_vm)};
  if (rc != JNI_OK) LOGE("Unable to get JVM. Error {}", rc);
  else LOGD("Got JVM");

  _cbRef = env->NewGlobalRef(cb);

  jclass clazz = env->GetObjectClass(cb);

  std::string signature{"(L" + std::string(JNI_RESULT_PATH) + ";)V"};
  _mid = env->GetMethodID(clazz, "onNativeEvent", signature.c_str());

  // Create thread
  _thread = std::thread(&AndroidFunktor::loop, this);
}

AndroidFunktor::~AndroidFunktor() {
  // Join thread
  {
    std::lock_guard<std::mutex> lock(_mut_r);
    _exit = true;
  }
  _cv.notify_one();
  if (_thread.joinable()) _thread.join();
}

/// \todo maybe don't crash the app if we have a dupe call
void AndroidFunktor::operator()(result_t const& r) {
  std::unique_lock<std::mutex> lock(_mut_r);

  if (_r) assert(false); // Duplicate calls are not allowed
  _r.emplace(r);
  lock.unlock();
  _cv.notify_one();
}

void AndroidFunktor::loop() {
  LOGD("Start Funktor thread");
  attach();
  while (true) {
    // Wait for event
    {
      std::unique_lock<std::mutex> lock(_mut_r);
      _cv.wait(lock, [this] { return _r.has_value() || _exit; });
    }
    if (_exit) break;
    call(*_r);
    // Push result to JVM
  }
  // Cleanup JNI environment before detaching
  LOGD("End Funktor thread");
  auto e{env()};
  if (e) e->DeleteGlobalRef(_cbRef);
  detach();
}

bool AndroidFunktor::attach() {
  auto e{env()};
  return _vm->AttachCurrentThread(&e, nullptr) == JNI_OK;
}

void AndroidFunktor::detach() { _vm->DetachCurrentThread(); }

void AndroidFunktor::call(result_t const& r) {
  LOGD("Call Callback");
  auto e{env()};
  if (e) e->CallVoidMethod(_cbRef, _mid, dispatch(e, r));
  _r.reset();
}

JNIEnv* AndroidFunktor::env() {
  JNIEnv* env{nullptr};
  auto rc{_vm->GetEnv((void**)&env, JNI_VERSION_1_6)};
  if (rc != JNI_OK) {
    LOGE("Failed to get JNIEnv. Error {}", rc);
    if (env) LOGD("But got JNIEnv anyway");
    else LOGE("AND Failed to get JNIEnv.");
    return nullptr;
  }
  return env;
}

} // namespace internal
