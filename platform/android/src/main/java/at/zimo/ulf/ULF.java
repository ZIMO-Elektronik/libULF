/**
 * 
 * ULF
 * 
 * 08.09.2026
 * 
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
 */

package at.zimo.ulf;

/**
 * The ULF API interface. This is tightly coupled to the 
 * {@link at.zimo.ulf.ULFAdapter} class, as it is only a 
 * slightly nicer wrapper to avoid flinging typeless handles 
 * around. 
 * 
 * Since the API consists of "a vew" methods, they are grouped to 
 * match the protocol they are using.
 * 
 * These Groups are currently: 
 *    - com
 *    - susiv2
 *    - mdu_ein
 */
public class ULF {
  public ULF() {
    _lib = ULFAdapter.create();
  }

  /**
   * Destroys the owned handle and invalidates the object
   * 
   * Corresponds to ::libulf_destroy
   */
  public void cleanup() {
    validateState();
    ULFAdapter.destroy(_lib);
    _lib = 0; 
  }

  /**
   * Initializes the USB layer
   * 
   * Corresponds to ::libulf_init
   * 
   * @return Error or 0
   */
  public int init() {
    return ULFAdapter.init(_lib);
  }

  /**
   * Opens the first USB device mathing the given identifiers
   * 
   * Corresponds to ::libulf_open 
   * 
   * @return Error or 0
   */
  public int open(int vid, int pid) {
    return ULFAdapter.open(_lib, vid, pid);
  }

  /**
   * Opens a the given USB device via its file descriptor
   * 
   * Corresponds to ::libulf_openFd
   * 
   * @return Error or 0
   */
  public int openFd(int Fd) {
    return ULFAdapter.openFd(_lib, Fd);
  }

  /**
   * Closes the open USB device 
   * 
   * Corresponds to ::libulf_close
   * 
   * @return Error or 0
   */
  public int close() {
    return ULFAdapter.close(_lib);
  }

  private long _lib;

  /**
   * Returns the COM API interface group
   */
  public COM com() {
    return com;
  }
  public final COM com = new COM();

  /**
   * The COM protocol API group, represented by this class. 
   */
  public final class COM {
    private COM() {}

    /**
     * Send a PING command to the USB device
     * 
     * Corresponds to ::libulf_com_ping
     * 
     * @return Response string
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public String ping() {
      validateState();
      return ULFAdapter.comPing(_lib);
    }

    /**
     * Send a RESET command to the USB device
     * 
     * Corresponds to ::libulf_com_reset
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean reset() {
      validateState();
      return ULFAdapter.comReset(_lib);
    }

    /**
     * Send a SUSIV2 command to the USB device
     * 
     * Corresponds to ::libulf_com_susiv2
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean susiv2() {
      validateState();
      return ULFAdapter.comSusiv2(_lib);
    }

    /**
     * Send a MDU_EIN command to the USB device
     * 
     * Corresponds to ::libulf_com_mdu_ein
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean mdu_ein() {
      validateState();
      return ULFAdapter.comMduEin(_lib);
    }
  }

  /**
   * Returns the SUSIV2 API group
   */
  public SUSIV2 susiv2() {
    return susiv2; 
  }
  public final SUSIV2 susiv2 = new SUSIV2();

  /**
   * The SUSIV2 protocol API group, represented by this class. 
   */
  public final class SUSIV2 {
    private SUSIV2() {}

    /**
     * Reads a CV from the decoder
     * 
     * Corresponds to ::libulf_susiv2_cv_read
     * 
     * @param cv Address of the CV (CV - 1)
     * 
     * @return CV value
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public int cvRead(int cv) {
      validateState();
      return ULFAdapter.susiv2CvRead(_lib, cv);
    }

    /**
     * Writes a CV to the decoder
     * 
     * Corresponds to ::libulf_susiv2_cv_write
     * 
     * @param cv  Address of the CV (CV - 1)
     * @param val Value to write
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean cvWrite(int cv, int val) {
      validateState();
      return ULFAdapter.susiv2CvWrite(_lib, cv, val);
    }

    /**
     * Erases the sound flash of the decoder
     * 
     * Corresponds to ::libulf_susiv2_zpp_erase
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zppErase() {
      validateState();
      return ULFAdapter.susiv2ZppErase(_lib);
    }

    /**
     * Writes a sound flash block to the decoder
     * 
     * Corresponds to ::libulf_susiv2_zpp_write
     * 
     * The flash blocks have to be written in ascending index order. 
     * 
     * @param zpp   A valid ZPP instance
     * @param index Flash block index
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zppWrite(ZPP zpp, int index) {
      validateState();
      return ULFAdapter.susiv2ZppWrite(_lib, zpp.get(), index);
    }

    /**
     * Requests decoder features
     * 
     * Corresponds to ::libulf_susiv2_features
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean features() {
      validateState();
      return ULFAdapter.susiv2Features(_lib);
    }

    /**
     * Requests the decoder to quit ZUSI mode
     * 
     * Corresponds to ::libulf_susiv2_exit
     * 
     * @param reset     Reset decoder
     * @param cv8_reset CV8 Reset
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean exit(boolean reset, boolean cv8_reset) {
      validateState();
      return ULFAdapter.susiv2Exit(_lib, reset, cv8_reset);
    }

    /**
     * Checks, if the load code on the decoder is valid for the developer 
     * code of the given ZPP
     * 
     * Corresponds to ::libulf_susiv2_zpp_lc_dc_query
     * 
     * @param zpp A valid ZPP instance
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zppLcDcQuery(ZPP zpp) {
      validateState();
      return ULFAdapter.susiv2ZppLcDcQuery(_lib, zpp.get());
    }
  }

  /**
   * Returns the MDU_EIN API group
   */
  public MDU_EIN mdu_ein() {
    return mdu_ein; 
  }
  public final MDU_EIN mdu_ein = new MDU_EIN();

  /**
   * The MDU_EIN protocol API group, represented by this class.
   */
  public class MDU_EIN {
    private MDU_EIN() {}

    /**
     * Commands all decoders to enter MDU mode (Update)
     * 
     * Corresponds to ::libulf_mdu_ein_enter_mdu
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean enterMDU() {
      validateState();
      return ULFAdapter.mduEinEnterMdu(_lib);
    }

    /**
     * Commands selected decoders to enter MDU mode (Update)
     * 
     * Corresponds to ::libulf_mdu_ein_enter_dcc_zsu
     * 
     * @param id    Decoder ID
     * @param sn    Decoder Serial number
     * @param done  No more pairs to enter
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean enterDCCZSU(long id, long sn, boolean done) {
      validateState();
      return ULFAdapter.mduEinEnterDccZsu(_lib, id, sn, done);
    } 

    /**
     * Commands selected decoders to enter MDU mode (SoundLoad)
     * 
     * Corresponds to ::libulf_mdu_ein_enter_dcc_zpp
     * 
     * @param id    Decoder ID
     * @param done  No more IDs to enter
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean enterDCCZPP(long id, boolean done) {
      validateState();
      return ULFAdapter.mduEinEnterDccZpp(_lib, id, done);
    }

    /**
     * Pings decoder(-s)
     * 
     * Corresponds to ::libulf_mdu_ein_ping
     * 
     * @param sn Decoder Serial Number
     * @param id Decoder ID
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean ping(long sn, long id) {
      validateState();
      return ULFAdapter.mduEinPing(_lib, sn, id);
    }

    /**
     * Pings all decoders
     * 
     * Corresponds to ::libulf_mdu_ein_ping_all
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean pingAll() {
      validateState();
      return ULFAdapter.mduEinPingAll(_lib);
    }

    /**
     * Configures transfer rate for decoder and USB device
     * 
     * Corresponds to ::libulf_mdu_ein_config_transfer_rate
     * 
     * @param transfer_rate Transfer Rate to set
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean configTransferRate(TransferRate transfer_rate) {
      validateState();
      return ULFAdapter.mduEinConfigTransferRate(_lib, transfer_rate.value());
    }

    /**
     * Reads a CV from the decoder
     * 
     * Corresponds to ::libulf_mdu_ein_cv_read
     * 
     * @param cv CV address
     * 
     * @return Value of the CV
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public int cvRead(int cv) {
      validateState();
      return ULFAdapter.mduEinCvRead(_lib, cv);
    }

    /** 
     * Writes a CV to the decoder
     * 
     * Corresponds to ::libulf_mdu_ein_cv_write
     * 
     * @param cv  CV address
     * @param val CV Value
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean cvWrite(int cv, int val) {
      validateState();
      return ULFAdapter.mduEinCvWrite(_lib, cv, val);
    }

    /**
     * Checks if the decoder is busy
     * 
     * Corresponds to ::libulf_mdu_ein_busy
     * 
     * @return True, if busy
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean busy() {
      validateState();
      return ULFAdapter.mduEinBusy(_lib);
    }

    /**
     * Checks if the given ZPP can fit into the decoder
     * 
     * Corresponds to ::libulf_mdu_ein_zpp_valid_query
     * 
     * @param zpp A valid ZPP instance
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zppValidQuery(ZPP zpp) {
      validateState();
      return ULFAdapter.mduEinZppValidQuery(_lib, zpp.get());
    }

    /**
     * Checks, if the load code on the decoder is valid for the developer 
     * code of the given ZPP
     * 
     * @param zpp A valid ZPP instance
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zppLcDcQuery(ZPP zpp) {
      validateState();
      return ULFAdapter.mduEinZppLcDcQuery(_lib, zpp.get());
    }

    /**
     * Erases the sound flash of the decoder
     * 
     * Corresponds to ::libulf_mdu_ein_zpp_erase
     * 
     * @param zpp A valid ZPP instance
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zppErase(ZPP zpp) {
      validateState();
      return ULFAdapter.mduEinZppErase(_lib, zpp.get());
    }

    /**
     * Writes a sound flash block to the decoder
     * 
     * Corresponds to ::libulf_mdu_ein_zpp_update
     * 
     * The flash blocks have to be written in ascending index order
     * 
     * @param zpp   A valid ZPP instance
     * @param index Flash block index
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zppUpdate(ZPP zpp, int index) {
      validateState();
      return ULFAdapter.mduEinZppUpdate(_lib, zpp.get(), index);
    }

    /**
     * Semantic end of the sound flash update
     * 
     * Corresponds to ::libulf_mdu_ein_zpp_update_end
     * 
     * @param zpp A valid ZPP instance
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zppUpdateEnd(ZPP zpp) {
      validateState();
      return ULFAdapter.mduEinZppUpdateEnd(_lib, zpp.get());
    }

    /**
     * Command the decoder to exit MDU and reset
     * 
     * Corresponds to ::libulf_mdu_ein_zpp_exit_reset
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zppExitReset() {
      validateState();
      return ULFAdapter.mduEinZppExitReset(_lib);
    }

    /**
     * Initializes the Salsa20 encryption
     * 
     * Corresponds to ::libulf_mdu_ein_zsu_salsa_20_iv
     * 
     * @param fw A valid Firmware instance
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zsuSalsa20Iv(Firmware fw) {
      validateState();
      return ULFAdapter.mduEinZsuSalsa20Iv(_lib, fw.zsu().get(), fw.index());
    }

    /**
     * Erases the firmware flash of the decoder
     * 
     * Corresponds to ::libulf_mdu_ein_zsu_erase
     * 
     * @param fw A valid Firmware instance
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zsuErase(Firmware fw) {
      validateState();
      return ULFAdapter.mduEinZsuErase(_lib, fw.zsu().get(), fw.index());
    }

    /**
     * Writes firmware flash block to the decoder
     * 
     * Corresponds to ::libulf_mdu_ein_zsu_update
     * 
     * @param fw    A valid Firmware instance
     * @param index Flash block index
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zsuUpdate(Firmware fw, int index) {
      validateState();
      return ULFAdapter.mduEinZsuUpdate(_lib, fw.zsu().get(), fw.index(), index);
    }

    /**
     * Starts the firmware flash verification (CRC32)
     * 
     * Corresponds to ::libulf_mdu_ein_zsu_crc32_start
     * 
     * @param fw A valid Firmware instance
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zsuCrc32Start(Firmware fw) {
      validateState();
      return ULFAdapter.mduEinZsuCrc32Start(_lib, fw.zsu().get(), fw.index());
    }

    /**
     * Checks the result of the firmware flash verification
     * 
     * Corresponds to ::libulf_mdu_ein_zsu_crc32_result
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zsuCrc32Result() {
      validateState();
      return ULFAdapter.mduEinZsuCrc32Result(_lib);
    }

    /**
     * Checks the result of the firmware flash verification and commands the 
     * decoder to leave MDU mode
     * 
     * Corresponds to ::libulf_mdu_ein_resul_zsu_crc32_result_exit
     * 
     * @return True, if successful
     * 
     * @throws IllegalStateException  If the object has been invalidated
     * @throws LibULFException        If the operation produced an Error
     */
    public boolean zsuCrc32ResultExit() {
      validateState();
      return ULFAdapter.mduEinZsuCrc32ResultExit(_lib);
    }
  }

  /**
   * Checks if the underlying handle was deleted
   * 
   * @throws IllegalStateException If the object was invalidated using cleanup
   */
  private void validateState() {
    if (_lib == 0) throw new IllegalStateException("LibULF was closed");
  }
}