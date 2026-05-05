/**
 * JNI LibKLUG interface
 *
 * \file    src/libklug/libklug_jni.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include <jni.h>
#include "libklug/internal/platform/android/jni_defines.hpp"

extern "C" {

jint JNI_OnLoad(JavaVM* vm, void* reserved);

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

JNI_METHOD(jlong, bridge_1create);
JNI_METHOD(void, bridge_1destroy, jlong handle);
JNI_METHOD(void, bridge_1register_1cb, jlong handle, jobject cb);
JNI_METHOD(void, bridge_1deregister_1cb, jlong handle);
JNI_METHOD(jobject, bridge_1result, jlong handle);
JNI_METHOD(jint, bridge_1init, jlong handle);
JNI_METHOD(
  jint, bridge_1open, jlong handle, jint vid = 0x1FC9u, jint pid = 0x81C1u);
JNI_METHOD(jint, bridge_1openFd, jlong handle, jint Fd);
JNI_METHOD(jint, bridge_1config, jlong handle);
JNI_METHOD(jint, bridge_1claim, jlong handle);
JNI_METHOD(jint, bridge_1release, jlong handle);
JNI_METHOD(void, bridge_1close, jlong handle);

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1com_1ping, jlong handle);
JNI_METHOD(jint, bridge_1com_1reset, jlong handle);
JNI_METHOD(jint, bridge_1com_1susiv2, jlong handle);
JNI_METHOD(jint, bridge_1com_1mdu_1ein, jlong handle);

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1susiv2_1cv_1read, jlong handle, jint cv);
JNI_METHOD(jint, bridge_1susiv2_1features, jlong handle);

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1mdu, jlong handle);
JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1dcc_1zsu, jlong handle);
JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1dcc_1zpp, jlong handle);
JNI_METHOD(jint, bridge_1mdu_1ein_1cv_1read, jlong handle, jint cv);
JNI_METHOD(jint, bridge_1mdu_1ein_1ping, jlong handle);
}
