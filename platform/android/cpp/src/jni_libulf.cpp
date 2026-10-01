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
 * JNI LibULF interface
 *
 * This is the native end of the JNI binding. Essentially, this wraps the C API
 * behind "Methods", which are linked from Java at runtime. This also means,
 * that the compiler cannot check definition mismatches between the native and
 * java side, so any mistake shows its effect at runtime.
 *
 * \file    platform/android/cpp/src/libulf_jni.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include <jni.h>
#include <array>

#include <ulf/c/libulf.h>
#include "inc/jni_context.hpp"
#include "inc/jni_defines.hpp"
#include "inc/jni_helper.hpp"
#include "inc/jni_libulf.hpp"

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

JNI_METHOD(jlong, create) { return to_jlong(libulf_create()); }

JNI_METHOD(void, destroy, jlong hlib) { libulf_destroy(to_lib(hlib)); }

JNI_METHOD(jint, init, jlong hlib) {
  return to_jint(libulf_init(to_lib(hlib)));
}

JNI_METHOD(jint, open, jlong hlib, jint vid, jint pid) {
  return to_jint(libulf_open(
    to_lib(hlib), static_cast<uint16_t>(vid), static_cast<uint16_t>(vid)));
}

JNI_METHOD(jint, openFd, jlong hlib, jint Fd) {
  return to_jint(libulf_openFd(to_lib(hlib), static_cast<int>(Fd)));
}

JNI_METHOD(jint, close, jlong hlib) {
  return to_jint(libulf_close(to_lib(hlib)));
}

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

JNI_METHOD(jstring, comPing, jlong hlib) {
  return stringFn(
    env, [&](auto b, auto l) { return libulf_com_ping(to_lib(hlib), b, l); });
}

JNI_METHOD(jboolean, comReset, jlong hlib) {
  return boolFn(env, [&](auto r) { return libulf_com_reset(to_lib(hlib), r); });
}

JNI_METHOD(jboolean, comSusiv2, jlong hlib) {
  return boolFn(env,
                [&](auto r) { return libulf_com_susiv2(to_lib(hlib), r); });
}

JNI_METHOD(jboolean, comMduEin, jlong hlib) {
  return boolFn(env,
                [&](auto r) { return libulf_com_mdu_ein(to_lib(hlib), r); });
}

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, susiv2CvRead, jlong hlib, jint cv) {
  return valueFn(env, [&](auto r) {
    return libulf_susiv2_cv_read(to_lib(hlib), static_cast<uint16_t>(cv), r);
  });
}

JNI_METHOD(jboolean, susiv2CvWrite, jlong hlib, jint cv, jint val) {
  return boolFn(env, [&](auto r) {
    return libulf_susiv2_cv_write(
      to_lib(hlib), static_cast<uint16_t>(cv), static_cast<uint8_t>(val), r);
  });
}

JNI_METHOD(jboolean, susiv2ZppErase, jlong hlib) {
  return boolFn(
    env, [&](auto r) { return libulf_susiv2_zpp_erase(to_lib(hlib), r); });
}

JNI_METHOD(jboolean, susiv2ZppWrite, jlong hlib, jlong hzpp, jint index) {
  return boolFn(env, [&](auto r) {
    return libulf_susiv2_zpp_write(
      to_lib(hlib), to_zpp(hzpp), static_cast<size_t>(index), r);
  });
}

JNI_METHOD(jboolean, susiv2Features, jlong hlib) {
  return boolFn(
    env, [&](auto r) { return libulf_susiv2_features(to_lib(hlib), r); });
}

JNI_METHOD(
  jboolean, susiv2Exit, jlong hlib, jboolean reboot, jboolean cv8_reset) {
  return boolFn(env, [&](auto r) {
    return libulf_susiv2_exit(
      to_lib(hlib), to_bool(reboot), to_bool(cv8_reset), r);
  });
}

JNI_METHOD(jboolean, susiv2ZppLcDcQuery, jlong hlib, jlong hzpp) {
  return boolFn(env, [&](auto r) {
    return libulf_susiv2_zpp_lc_dc_query(to_lib(hlib), to_zpp(hzpp), r);
  });
}

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

JNI_METHOD(jboolean, mduEinEnterMdu, jlong hlib) {
  return boolFn(
    env, [&](auto r) { return libulf_mdu_ein_enter_mdu(to_lib(hlib), r); });
}

JNI_METHOD(
  jboolean, mduEinEnterDccZsu, jlong hlib, jlong id, jlong sn, jboolean done) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_enter_dcc_zsu(to_lib(hlib),
                                        static_cast<uint32_t>(id),
                                        static_cast<uint32_t>(sn),
                                        to_bool(done),
                                        r);
  });
}

JNI_METHOD(jboolean, mduEinEnterDccZpp, jlong hlib, jint sn, jboolean done) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_enter_dcc_zpp(
      to_lib(hlib), static_cast<uint32_t>(sn), to_bool(done), r);
  });
}

JNI_METHOD(jboolean, mduEinPing, jlong hlib, jint sn, jint id) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_ping(
      to_lib(hlib), static_cast<uint32_t>(id), static_cast<uint32_t>(sn), r);
  });
}

JNI_METHOD(jboolean, mduEinPingAll, jlong hlib) {
  return boolFn(
    env, [&](auto r) { return libulf_mdu_ein_ping_all(to_lib(hlib), r); });
}

JNI_METHOD(jboolean, mduEinConfigTransferRate, jlong hlib, jint transfer_rate) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_config_transfer_rate(
      to_lib(hlib), static_cast<uint8_t>(transfer_rate), r);
  });
}

JNI_METHOD(jint, mduEinCvRead, jlong hlib, jint cv) {
  return valueFn(env, [&](auto r) {
    return libulf_mdu_ein_cv_read(to_lib(hlib), static_cast<uint16_t>(cv), r);
  });
}

JNI_METHOD(jboolean, mduEinCvWrite, jlong hlib, jint cv, jint val) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_cv_write(
      to_lib(hlib), static_cast<uint16_t>(cv), static_cast<uint8_t>(val), r);
  });
}

JNI_METHOD(jboolean, mduEinBusy, jlong hlib) {
  return boolFn(env,
                [&](auto r) { return libulf_mdu_ein_busy(to_lib(hlib), r); });
}

///

JNI_METHOD(jboolean, mduEinZppValidQuery, jlong hlib, jlong hzpp) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zpp_valid_query(to_lib(hlib), to_zpp(hzpp), r);
  });
}

JNI_METHOD(jboolean, mduEinZppLcDcQuery, jlong hlib, jlong hzpp) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zpp_lc_dc_query(to_lib(hlib), to_zpp(hzpp), r);
  });
}

JNI_METHOD(jboolean, mduEinZppErase, jlong hlib, jlong hzpp) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zpp_erase(to_lib(hlib), to_zpp(hzpp), r);
  });
}

JNI_METHOD(jboolean, mduEinZppUpdate, jlong hlib, jlong hzpp, jint index) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zpp_update(
      to_lib(hlib), to_zpp(hzpp), static_cast<size_t>(index), r);
  });
}

JNI_METHOD(jboolean, mduEinZppUpdateEnd, jlong hlib, jlong hzpp) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zpp_update_end(to_lib(hlib), to_zpp(hzpp), r);
  });
}

JNI_METHOD(jboolean, mduEinZppExitReset, jlong hlib) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zpp_exit_reset(to_lib(hlib), r);
  });
}

///

JNI_METHOD(
  jboolean, mduEinZsuSalsa20Iv, jlong hlib, jlong hzsu, jint fw_index) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zsu_salsa20_iv(
      to_lib(hlib), to_zsu(hzsu), static_cast<size_t>(fw_index), r);
  });
}

JNI_METHOD(jboolean, mduEinZsuErase, jlong hlib, jlong hzsu, jint fw_index) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zsu_erase(
      to_lib(hlib), to_zsu(hzsu), static_cast<size_t>(fw_index), r);
  });
}

JNI_METHOD(jboolean,
           mduEinZsuUpdate,
           jlong hlib,
           jlong hzsu,
           jint fw_index,
           jint index) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zsu_update(to_lib(hlib),
                                     to_zsu(hzsu),
                                     static_cast<size_t>(fw_index),
                                     static_cast<size_t>(index),
                                     r);
  });
}

JNI_METHOD(jboolean, mduEinCrc32Start, jlong hlib, jlong hzsu, jint fw_index) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zsu_crc32_start(
      to_lib(hlib), to_zsu(hzsu), static_cast<size_t>(fw_index), r);
  });
}

JNI_METHOD(jboolean, mduEinCrc32Result, jlong hlib) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zsu_crc32_result(to_lib(hlib), r);
  });
}

JNI_METHOD(jboolean, mduEinCrc32ResultExit, jlong hlib) {
  return boolFn(env, [&](auto r) {
    return libulf_mdu_ein_zsu_crc32_result_exit(to_lib(hlib), r);
  });
}

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

JNI_METHOD(jlong, zppRead, jstring path) {
  size_t const nativeLength{static_cast<size_t>(env->GetStringUTFLength(path))};
  char const* nativePath{env->GetStringUTFChars(path, nullptr)};
  if (!nativePath) return 0;

  auto hzpp{libulf_zpp_read(nativePath, nativeLength)};

  env->ReleaseStringUTFChars(path, nativePath);
  return to_jlong(hzpp);
}

JNI_METHOD(void, zppRelease, jlong hzpp) {
  return libulf_zpp_release(to_zpp(hzpp));
}

JNI_METHOD(jint, zppBlocks, jlong hzpp) {
  return static_cast<jint>(libulf_zpp_blocks(to_zpp(hzpp)));
}

JNI_METHOD(jstring, zppAuthor, jlong hzpp) {
  return env->NewStringUTF(libulf_zpp_author(to_zpp(hzpp)));
}

JNI_METHOD(jstring, zppEmail, jlong hzpp) {
  return env->NewStringUTF(libulf_zpp_email(to_zpp(hzpp)));
}

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

JNI_METHOD(jlong, zsuRead, jstring path) {
  size_t const nativeLength{static_cast<size_t>(env->GetStringUTFLength(path))};
  char const* nativePath{env->GetStringUTFChars(path, nullptr)};
  if (!nativePath) return 0;

  auto hzsu{libulf_zsu_read(nativePath, nativeLength)};

  env->ReleaseStringUTFChars(path, nativePath);
  return to_jlong(hzsu);
}

JNI_METHOD(void, zsuRelease, jlong hzsu) {
  return libulf_zsu_release(to_zsu(hzsu));
}

JNI_METHOD(jint, zsuGetFirmwareCount, jlong hzsu) {
  return static_cast<jint>(libulf_zsu_get_firmware_count(to_zsu(hzsu)));
}

JNI_METHOD(jlong, zsuGetFirmwareID, jlong hzsu, jint fw_index) {
  return static_cast<jint>(
    libulf_zsu_get_firmware_id(to_zsu(hzsu), static_cast<size_t>(fw_index)));
}

JNI_METHOD(jstring, zsuGetFirmwareName, jlong hzsu, jint fw_index) {
  return env->NewStringUTF(
    libulf_zsu_get_firmware_name(to_zsu(hzsu), static_cast<size_t>(fw_index)));
}

JNI_METHOD(jstring, zsuGetFirmwareMajorVersion, jlong hzsu, jint fw_index) {
  return env->NewStringUTF(libulf_zsu_get_firmware_major_version(
    to_zsu(hzsu), static_cast<size_t>(fw_index)));
}

JNI_METHOD(jstring, zsuGetFirmwareMinorVersion, jlong hzsu, jint fw_index) {
  return env->NewStringUTF(libulf_zsu_get_firmware_minor_version(
    to_zsu(hzsu), static_cast<size_t>(fw_index)));
}

JNI_METHOD(jint, zsuGetFirmwareType, jlong hzsu, jint fw_index) {
  return static_cast<jint>(
    libulf_zsu_get_firmware_type(to_zsu(hzsu), static_cast<size_t>(fw_index)));
}

JNI_METHOD(jint, zsuGetFirmwareBlockCount, jlong hzsu, jint fw_index) {
  return static_cast<jint>(libulf_zsu_get_firmware_block_count(
    to_zsu(hzsu), static_cast<size_t>(fw_index)));
}
