/**
 * Copyright (C) 2026 [ZIMO Elektronik]
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
 * JNI LibKLUG interface
 *
 * This is the native end of the JNI binding. Essentially, this wraps the C API
 * behind "Methods", which are linked from Java at runtime. This also means,
 * that the compiler cannot check definition mismatches between the native and
 * java side, so any mistake shows its effect at runtime.
 *
 * \note
 * Unsure, if this is needed, but its nicer to read at least
 *
 * \file    src/libklug/bridge_jni.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include <jni.h>
#include "jni_defines.hpp"

extern "C" {

jint JNI_OnLoad(JavaVM* vm, void* reserved);

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

JNI_METHOD(jlong, create);
JNI_METHOD(void, destroy, jlong hlib);
JNI_METHOD(jint, init, jlong hlib);
JNI_METHOD(jint, open, jlong hlib, jint vid, jint pid);
JNI_METHOD(jint, openFd, jlong hlib, jint Fd);
JNI_METHOD(jint, close, jlong hlib);

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

JNI_METHOD(jstring, comPing, jlong hlib);
JNI_METHOD(jboolean, comReset, jlong hlib);
JNI_METHOD(jboolean, comSusiv2, jlong hlib);
JNI_METHOD(jboolean, comMduEin, jlong hlib);

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

JNI_METHOD(jint, susiv2CvRead, jlong hlib, jint cv);
JNI_METHOD(jboolean, susiv2CvWrite, jlong hlib, jint cv, jint val);
JNI_METHOD(jboolean, susiv2ZppErase, jlong hlib);
JNI_METHOD(jboolean, susiv2ZppWrite, jlong hlib, jlong hzpp, jint index);
JNI_METHOD(jboolean, susiv2Features, jlong hlib);
JNI_METHOD(
  jboolean, susiv2Exit, jlong hlib, jboolean reboot, jboolean cv8_reset);
JNI_METHOD(jboolean, susiv2ZppLcDcQuery, jlong hlib, jlong hzpp);

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

JNI_METHOD(jboolean, mduEinEnterMdu, jlong hlib);
JNI_METHOD(
  jboolean, mduEinEnterDccZsu, jlong hlib, jlong id, jlong sn, jboolean done);
JNI_METHOD(jboolean, mduEinEnterDccZpp, jlong hlib, jlong sn, jboolean done);
JNI_METHOD(jboolean, mduEinPing, jlong hlib, jlong sn, jlong id);
JNI_METHOD(jboolean, mduEinPingAll, jlong hlib);
JNI_METHOD(jboolean, mduEinConfigTransferRate, jlong hlib, jint transfer_rate);
JNI_METHOD(jint, mduEinCvRead, jlong hlib, jint cv);
JNI_METHOD(jboolean, mduEinCvWrite, jlong hlib, jint cv, jint val);
JNI_METHOD(jboolean, mduEinBusy, jlong hlib);

JNI_METHOD(jboolean, mduEinZppValidQuery, jlong hlib, jlong hzpp);
JNI_METHOD(jboolean, mduEinZppLcDcQuery, jlong hlib, jlong hzpp);
JNI_METHOD(jboolean, mduEinZppErase, jlong hlib, jlong hzpp);
JNI_METHOD(jboolean, mduEinZppUpdate, jlong hlib, jlong hzpp, jint index);
JNI_METHOD(jboolean, mduEinZppUpdateEnd, jlong hlib, jlong hzpp);
JNI_METHOD(jboolean, mduEinZppExitReset, jlong hlib);

JNI_METHOD(jboolean, mduEinZsuSalsa20Iv, jlong hlib, jlong hzsu, jint fw_index);
JNI_METHOD(jboolean, mduEinZsuErase, jlong hlib, jlong hzsu, jint fw_index);
JNI_METHOD(jboolean, mduEinZsuUpdate, jlong hlib, jlong hzsu, jint fw_index);
JNI_METHOD(jboolean, mduEinCrc32Start, jlong hlib, jlong hzsu, jint fw_index);
JNI_METHOD(jboolean, mduEinCrc32Result, jlong hlib);
JNI_METHOD(jboolean, mduEinCrc32ResultExit, jlong hlib);

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

JNI_METHOD(jlong, zppRead, jstring path);
JNI_METHOD(void, zppRelease, jlong hzpp);
JNI_METHOD(jint, zppBlocks, jlong hzpp);
JNI_METHOD(jstring, zppAuthor, jlong hzpp);
JNI_METHOD(jstring, zppEmail, jlong zpp);

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

JNI_METHOD(jlong, zsuRead, jstring path);
JNI_METHOD(void, zsuRelease, jlong hzsu);
JNI_METHOD(jint, zsuGetFirmwareCount, jlong hzsu);
JNI_METHOD(jlong, zsuGetFirmwareID, jlong hzsu, jint fw_index);
JNI_METHOD(jstring, zsuGetFirmwareName, jlong hzsu, jint fw_index);
JNI_METHOD(jstring, zsuGetFirmwareMajorVersion, jlong hzsu, jint fw_index);
JNI_METHOD(jstring, zsuGetFirmwareMinorVersion, jlong hzsu, jint fw_index);
JNI_METHOD(jint, zsuGetFirmwareType, jlong hzsu, jint fw_index);
JNI_METHOD(jint, zsuGetFirmwareBlockCount, jlong hzsu, jint fw_index);
// Data
// Size
}
