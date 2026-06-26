/**
 * JNI LibKLUG interface
 *
 * \file    src/libklug/bridge_jni.hpp
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
JNI_METHOD(jint, bridge_1susiv2_1cv_1write, jlong handle, jint cv, jint value);
JNI_METHOD(jint, bridge_1susiv2_1zpp_1erase, jlong handle);
JNI_METHOD(
  jint, bridge_1susiv2_1zpp_1write, jlong handle, jlong zpp_handle, jint index);
JNI_METHOD(jint, bridge_1susiv2_1features, jlong handle);
JNI_METHOD(
  jint, bridge_1susiv2_1exit, jlong handle, jint reboot, jint cv8reset);
JNI_METHOD(jint, bridge_1susiv2_1lc_1dc_1query, jlong handle, jlong zpp_handle);

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

/// ---- MDU entry

JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1mdu, jlong handle);
JNI_METHOD(
  jint, bridge_1mdu_1ein_1enter_1dcc_1zsu, jlong id, jlong sn, jlong handle);
JNI_METHOD(jint, bridge_1mdu_1ein_1enter_1dcc_1zpp, jlong sn, jlong handle);

/// --- General

JNI_METHOD(jint, bridge_1mdu_1ein_1ping, jlong sn, jlong handle);
JNI_METHOD(jint, bridge_1mdu_1ein_1ping_1all, jlong handle);
JNI_METHOD(jint,
           bridge_1mdu_1ein_1config_1transfer_1rate,
           jlong handle,
           jint transfer_rate);
JNI_METHOD(jint, bridge_1mdu_1ein_1cv_1read, jlong handle, jint cv);
JNI_METHOD(
  jint, bridge_1mdu_1ein_1cv_1write, jlong handle, jint cv, jint value);
JNI_METHOD(jint, bridge_1mdu_1ein_1busy, jlong handle);

/// ---- MDU ZPP

JNI_METHOD(jint,
           bridge_1mdu_1ein_1zpp_1valid_1query,
           jlong handle,
           jlong zpp_handle);
JNI_METHOD(jint,
           bridge_1mdu_1ein_1zpp_1lc_1dc_1query,
           jlong handle,
           jlong zpp_handle);
JNI_METHOD(jint, bridge_1mdu_1ein_1zpp_1erase, jlong handle, jlong zpp_handle);
JNI_METHOD(jint,
           bridge_1mdu_1ein_1zpp_1update,
           jlong handle,
           jlong zpp_handle,
           jint index);
JNI_METHOD(jint,
           bridge_1mdu_1ein_1zpp_1update_1end,
           jlong handle,
           jlong zpp_handle);
JNI_METHOD(jint, bridge_1mdu_1ein_1zpp_1exit_1reset, jlong handle);

/// ---- MDU ZSU

JNI_METHOD(jint,
           bridge_1mdu_1ein_1zsu_1salsa20_1iv,
           jlong handle,
           jlong firmware_iterator_handle);
JNI_METHOD(jint,
           bridge_1mdu_1ein_1zsu_1erase,
           jlong handle,
           jlong firmware_iterator_handle);
JNI_METHOD(jint,
           bridge_1mdu_1ein_1zsu_1update,
           jlong handle,
           jlong firmware_iterator_handle);
JNI_METHOD(jint,
           bridge_1mdu_1ein_1zsu_1crc32_1start,
           jlong handle,
           jlong firmware_iterator_handle);
JNI_METHOD(jint, bridge_1mdu_1ein_1zsu_1crc32_1result, jlong handle);
JNI_METHOD(jint, bridge_1mdu_1ein_1zsu_1crc32_1result_1end, jlong handle);

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

/// ---- ZPP lifetime

JNI_METHOD(jlong, libklug_1zpp_1read, jstring path);
JNI_METHOD(void, libklug_1zpp_1release, jlong zpp_handle);

/// ---- ZPP Ops

JNI_METHOD(jlong, libklug_1zpp_1blocks, jlong zpp_handle);
JNI_METHOD(jstring, libklug_1zpp_1author, jlong zpp_handle);
JNI_METHOD(jstring, libklug_1zpp_1email, jlong zpp_handle);

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

/// ---- ZSU lifetime

JNI_METHOD(jlong, libklug_1zsu_1read, jstring path);
JNI_METHOD(void, libklug_1zsu_1release, jlong zsu_handle);

/// ---- Iterator lifetime

JNI_METHOD(jlong,
           libklug_1zsu_1firmware_1iterator_1create_1begin,
           jlong zsu_handle);
JNI_METHOD(jlong,
           libklug_1zsu_1firmware_1iterator_1create_1end,
           jlong zsu_handle);
JNI_METHOD(void,
           libklug_zsu_destroy_firmware_iterator,
           jlong firmware_iterator_handle);

/// ---- Iterator ops

JNI_METHOD(jint,
           libklug_1zsu_1firmware_1iterator_1next,
           jlong firmware_iterator_handle);
JNI_METHOD(jint,
           libklug_1zsu_1firmware_1iterator_1previous,
           jlong firmware_iterator_handle);
JNI_METHOD(jint,
           libklug_1zsu_1firmware_1iterator_1equals,
           jlong lhs,
           jlong rhs);

/// ---- Firmware ops

JNI_METHOD(jlong,
           libklug_1zsu_1firmware_1iterator_1get_1id,
           jlong firmware_iterator_handle);
JNI_METHOD(jstring,
           libklug_1zsu_1firmware_1iterator_1get_1name,
           jlong firmware_iterator_handle);
JNI_METHOD(jstring,
           libklug_1zsu_1firmware_1iterator_1get_1version_1major,
           jlong firmware_iterator_handle);
JNI_METHOD(jstring,
           libklug_1zsu_1firmware_1iterator_1get_1version_1minor,
           jlong firmware_iterator_handle);
JNI_METHOD(jint,
           libklug_1zsu_1firmware_1iterator_1get_1type,
           jlong firmware_iterator_handle);
JNI_METHOD(jlong,
           libklug_1zsu_1firmware_1iterator_1get_1blocks,
           jlong firmware_iterator_handle);
}
