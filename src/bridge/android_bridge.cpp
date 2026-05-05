/**
 * JNI bridge (for android)
 *
 * \file    src/bridge/android_bridge.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include <jni.h>
#include <array>

#include "android_result.hpp"
#include "bridge/bridge.hpp"
#include "bridge/internal/bridge.hpp"
#include "bridge/jni_defines.hpp"
#include "internal/android_funktor.hpp"
#include "internal/jni_context.hpp"

extern "C" {

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

JNI_METHOD(jlong, bridge_1create) { return (jlong)bridge_create(); }

JNI_METHOD(void, bridge_1destroy, jlong handle) {
  return bridge_destroy(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(void, bridge_1register_1cb, jlong handle, jobject cb) {
  return reinterpret_cast<bridge::Bridge*>(handle)->registerCB(
    std::make_unique<internal::AndroidFunktor>(env, thiz, cb));
}

JNI_METHOD(void, bridge_1deregister_1cb, jlong handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->deregisterCB();
}

JNI_METHOD(jobject, bridge_1result, jlong handle) {
  auto const r{bridge_result(reinterpret_cast<bridge_handle>(handle))};
  return dispatch(env, r);
}

JNI_METHOD(jint, bridge_1init, jlong handle) {
  return bridge_init(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(
  jint, bridge_1open, jlong handle, jint vid = 0x1FC9u, jint pid = 0x81C1u) {
  return bridge_open(reinterpret_cast<bridge_handle>(handle), vid, pid);
}

JNI_METHOD(jint, bridge_1openFd, jlong handle, jint Fd) {
  return bridge_openFd(reinterpret_cast<bridge_handle>(handle), Fd);
}

JNI_METHOD(jint, bridge_1config, jlong handle) {
  return bridge_config(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1claim, jlong handle) {
  return bridge_claim(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1release, jlong handle) {
  return bridge_release(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(void, bridge_1close, jlong handle) {
  return bridge_close(reinterpret_cast<bridge_handle>(handle));
}

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1com_1ping, jlong handle) {
  return bridge_com_ping(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1com_1reset, jlong handle) {
  return bridge_com_reset(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1com_1susiv2, jlong handle) {
  return bridge_com_susiv2(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1com_1mdu_1ein, jlong handle) {
  return bridge_com_mdu_ein(reinterpret_cast<bridge_handle>(handle));
}

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1susiv2_1cv_1read, jlong handle, jint cv) {
  return bridge_susiv2_cv_read(reinterpret_cast<bridge_handle>(handle), cv);
}

JNI_METHOD(jint, bridge_1susiv2_1features, jlong handle) {
  return bridge_susiv2_features(reinterpret_cast<bridge_handle>(handle));
}

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1mdu, jlong handle) {
  return bridge_mdu_ein_enter_mdu(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1dcc_1zsu, jlong handle) {
  return bridge_mdu_ein_enter_dcc_zsu(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1dcc_1zpp, jlong handle) {
  return bridge_mdu_ein_enter_dcc_zpp(reinterpret_cast<bridge_handle>(handle));
}

JNI_METHOD(jint, bridge_1mdu_1ein_1cv_1read, jlong handle, jint cv) {
  return bridge_mdu_ein_cv_read(reinterpret_cast<bridge_handle>(handle), cv);
}

JNI_METHOD(jint, bridge_1mdu_1ein_1ping, jlong handle) {
  return bridge_mdu_ein_ping(reinterpret_cast<bridge_handle>(handle));
}
}
