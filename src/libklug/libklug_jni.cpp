/**
 * JNI LibKLUG interface
 *
 * \file    src/libklug/libklug_jni.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include <jni.h>
#include <array>

#include "libklug/internal/platform/android/jni_context.hpp"
#include "libklug/internal/platform/android/jni_defines.hpp"
#include "libklug/internal/platform/android/jni_dispatch.hpp"
#include "libklug/internal/platform/android/jni_functor.hpp"
#include "libklug/libklug.hpp"
#include "libklug/libklug_jni.hpp"

/**
 * Cast helper
 *
 */
constexpr auto to_bridge(jlong handle) {
  return reinterpret_cast<bridge::Bridge*>(handle);
}

/**
 * Overload JNI OnLoad to create class context
 *
 * \param vm JVM
 * \param reserved IDFK
 * \return jint JNI_VERSION_1_6
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  std::construct_at(&internal::jni_ctx, vm);
  return JNI_VERSION_1_6;
}

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

JNI_METHOD(jlong, bridge_1create) {
  return reinterpret_cast<jlong>(new bridge::Bridge());
}

JNI_METHOD(void, bridge_1destroy, jlong handle) {
  return delete to_bridge(handle);
}

JNI_METHOD(void, bridge_1register_1cb, jlong handle, jobject cb) {
  return to_bridge(handle)->registerCB(
    std::make_unique<internal::AndroidFunctor>(env, thiz, cb));
}

JNI_METHOD(void, bridge_1deregister_1cb, jlong handle) {
  return to_bridge(handle)->deregisterCB();
}

JNI_METHOD(jobject, bridge_1result, jlong handle) {
  return res::jni_dispatch(env, to_bridge(handle)->result());
}

JNI_METHOD(jint, bridge_1init, jlong handle) {
  return to_bridge(handle)->init();
}

JNI_METHOD(jint, bridge_1open, jlong handle, jint vid, jint pid) {
  return to_bridge(handle)->open(vid, pid);
}

JNI_METHOD(jint, bridge_1openFd, jlong handle, jint Fd) {
  return to_bridge(handle)->openFd(Fd);
}

JNI_METHOD(jint, bridge_1config, jlong handle) {
  return to_bridge(handle)->config();
}

JNI_METHOD(jint, bridge_1claim, jlong handle) {
  return to_bridge(handle)->claim();
}

JNI_METHOD(jint, bridge_1release, jlong handle) {
  return to_bridge(handle)->release();
}

JNI_METHOD(void, bridge_1close, jlong handle) {
  return to_bridge(handle)->close();
}

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1com_1ping, jlong handle) {
  return to_bridge(handle)->com().ping();
}

JNI_METHOD(jint, bridge_1com_1reset, jlong handle) {
  return to_bridge(handle)->com().reset();
}

JNI_METHOD(jint, bridge_1com_1susiv2, jlong handle) {
  return to_bridge(handle)->com().susiv2();
}

JNI_METHOD(jint, bridge_1com_1mdu_1ein, jlong handle) {
  return to_bridge(handle)->com().mdu_ein();
}

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1susiv2_1cv_1read, jlong handle, jint cv) {
  return to_bridge(handle)->susiv2().cvRead(cv);
}

JNI_METHOD(jint, bridge_1susiv2_1features, jlong handle) {
  return to_bridge(handle)->susiv2().features();
}

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1mdu, jlong handle) {
  return to_bridge(handle)->mdu_ein().enterMDU();
}

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1dcc_1zsu, jlong handle) {
  return to_bridge(handle)->mdu_ein().enterDCCZSU();
}

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1dcc_1zpp, jlong handle) {
  return to_bridge(handle)->mdu_ein().enterDCCZPP();
}

JNI_METHOD(jint, bridge_1mdu_1ein_1cv_1read, jlong handle, jint cv) {
  return to_bridge(handle)->mdu_ein().cvRead(cv);
}

JNI_METHOD(jint, bridge_1mdu_1ein_1ping, jlong handle) {
  return to_bridge(handle)->mdu_ein().ping();
}
