/**
 * 
 * KLUGAdapter
 * 
 * 08.09.2026
 * 
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
 */

package at.zimo.klug;

/**
 * An Adapter class for the JNI native bridge. This class connects to 
 * the corresponding native JNI functions at runtime. This also means, 
 * that if something is mis-defined or similar, this has its effect only 
 * at runtime. 
 * 
 * THIS CLASS IS NOT INTENDED FOR USE, USE KLUG INSTEAD!
 */
public class KLUGAdapter {
  static {
    System.loadLibrary("android_wrapper");
  }

  /** ----------------------------------------------------------------
   *  Create/Configure/Destroy
   *  ----------------------------------------------------------------
   */ 

  static native long create(); 
  static native void destroy(long hlib);
  static native int init(long hlib);
  static native int open(long hlib, int vid, int pid); 
  static native int openFd(long hlib, int fd); 
  static native int close(long hlib);

  /** ----------------------------------------------------------------
   *  Bridge COM
   *  ----------------------------------------------------------------
   */ 

  static native String comPing(long hlib);
  static native boolean comReset(long hlib);
  static native boolean comSusiv2(long hlib);
  static native boolean comMduEin(long hlib);

  /** ---------------------------------------------------
   *  Bridge SUSIV2
   *  ---------------------------------------------------
   */

  static native int susiv2CvRead(long hlib, int cv);
  static native boolean susiv2CvWrite(long hlib, int cv, int val);
  static native boolean susiv2ZppErase(long hlib);
  static native boolean susiv2ZppWrite(long hlib, long hzpp, int index);
  static native boolean susiv2Features(long hlib);
  static native boolean susiv2Exit(long hlib, boolean reset, boolean cv8_reset);
  static native boolean susiv2ZppLcDcQuery(long hlib, long hzpp);

  /** ---------------------------------------------------
   *  Bridge MDU_EIN
   *  ---------------------------------------------------
   */

  static native boolean mduEinEnterMdu(long hlib);
  static native boolean mduEinEnterDccZsu(long hlib, long id, long sn, boolean done);
  static native boolean mduEinEnterDccZpp(long hlib, long sn, boolean done);
  static native boolean mduEinPing(long hlib, long sn, long id);
  static native boolean mduEinPingAll(long hlib);
  static native boolean mduEinConfigTransferRate(long hlib, int transfer_rate);
  static native int mduEinCvRead(long hlib, int cv);
  static native boolean mduEinCvWrite(long hlib, int cv, int val);
  static native boolean mduEinBusy(long hlib); 

  static native boolean mduEinZppValidQuery(long hlib, long hzpp);
  static native boolean mduEinZppLcDcQuery(long hlib, long hzpp);
  static native boolean mduEinZppErase(long hlib, long hzpp);
  static native boolean mduEinZppUpdate(long hlib, long hzpp, int index);
  static native boolean mduEinZppUpdateEnd(long hlib, long hzpp);
  static native boolean mduEinZppExitReset(long hlib);

  static native boolean mduEinZsuSalsa20Iv(long hlib, long hzsu, int fw_index);
  static native boolean mduEinZsuErase(long hlib, long hzsu, int fw_index);
  static native boolean mduEinZsuUpdate(long hlib, long hzsu, int fw_index, int index);
  static native boolean mduEinZsuCrc32Start(long hlib, long hzsu, int fw_index);
  static native boolean mduEinZsuCrc32Result(long hlib);
  static native boolean mduEinZsuCrc32ResultExit(long hlib);

  /** ---------------------------------------------------
   *  Bridge ZPP
   *  ---------------------------------------------------
   */

  static native long zppRead(String path); 
  static native void zppRelease(long hzpp);
  static native int zppBlocks(long hzpp);
  static native String zppAuthor(long hzpp);
  static native String zppEmail(long hzpp);

  /** ---------------------------------------------------
   *  Bridge ZSU
   *  ---------------------------------------------------
   */

  static native long zsuRead(String path);
  static native void zsuRelease(long hzsu);
  static native int zsuGetFirmwareCount(long hzsu);
  static native long zsuGetFirmwareId(long hzsu, int fw_index);
  static native String zsuGetFirmwareName(long hzsu, int fw_index);
  static native String zsuGetFirmwareMajorVersion(long hzsu, int fw_index);
  static native String zsuGetFirmwareMinorVersion(long hzsu, int fw_index);
  static native int zsuGetFirmwareType(long hzsu, int fw_index);
  static native int zsuGetFirmwareBlockCount(long hzsu, int fw_index);
}
