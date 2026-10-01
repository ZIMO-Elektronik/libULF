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
 * libULF API
 *
 * \file    include/ulf/c/libulf.h
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#ifndef LIBULF_H
#define LIBULF_H

#ifdef __cplusplus
#  include <cstddef>
#  include <cstdint>
extern "C" {
#else
#  include <stdbool.h>
#  include <stddef.h>
#  include <stdint.h>
#endif

#include "error.h"

// Opaque poninters
typedef struct libulf_instance* libulf_handle;
typedef struct zpp_instance* zpp_handle;
typedef struct zsu_instance* zsu_handle;

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

// --- Lifetime --- //

/**
 * Create a libulf object
 *
 * \details
 * Allocated libulf object is made available using an opaque pointer
 *
 * \warning
 * A created libulf object MUST be destroyed using `libulf_destroy` to avoid a
 * memory leak. The user is responsible for keeping the handle alive until then.
 *
 * \return libulf_handle
 */
libulf_handle libulf_create();

/**
 * Destroy a libulf object
 *
 * \details
 * Deallocates a previously created libulf object using the given handle.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param handle libulf handle
 */
void libulf_destroy(libulf_handle handle);

// --- Connection Specifics --- //

/**
 * Init
 *
 * \details
 * Initilaizes the USB backend
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param handle libulf handle
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error libulf_init(libulf_handle handle);

/**
 * Open device
 *
 * \details
 * Attempts to open the first device matching the given identifiers
 *
 * \note
 * Depending on the platform, not all necessary permissions may be present. For
 * example on Android, use of this function is discouraged, as it wont succeed
 * unless the device is rooted. See README.md for more details
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param handle  libulf handle
 * \param vid     Device VID
 * \param pid     Device PID
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error libulf_open(libulf_handle handle, uint16_t vid, uint16_t pid);

/**
 * Open device by file descriptor
 *
 * \details
 * More of a wrapper, this "opens" a device
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param handle  libulf handle
 * \param Fd      File descriptor
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error libulf_openFd(libulf_handle handle, int Fd);

/**
 * Close device
 *
 * \details
 * Closes the device
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param handle  libulf handle
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error libulf_close(libulf_handle handle);

/**
 * Returns the last error string
 *
 * \warning
 * Instance save, but not thread save.
 *
 * \param handle
 * \return char const*
 */
char const* libulf_last_error_string();

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

/**
 * Ping device
 *
 * \details
 * Performs an [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) Ping.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] handle  libulf handle
 * \param [out]   buf     The buffer to copy the result string to
 * \param [inout] len     The size of the buffer. On success, `len` will contain
 *                        the actual byte count copied
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error libulf_com_ping(libulf_handle hlib, char* buf, size_t* len);

/**
 * Reset device
 *
 * \details
 * Performs an [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) Reset.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] handle  libulf handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error libulf_com_reset(libulf_handle hlib, bool* success);

/**
 * Enter SUSIV2 Mode
 *
 * \details
 * Performs an [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) SUSIV2.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] handle  libulf handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error libulf_com_susiv2(libulf_handle hlib, bool* success);

/**
 * Enter MDU_EIN Mode
 *
 * \details
 * Performs an [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) MDU_EIN.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] handle  libulf handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error libulf_com_mdu_ein(libulf_handle hlib, bool* success);

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

/**
 * CvRead
 *
 * \details
 * Read a single Cv using
 * [SUSIV2](https://github.com/ZIMO-Elektronik/ULF_SUSIV2)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] handle  libulf handle
 * \param [in]    cv      Cv address (zero-based, meaning Cv - 1)
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error
libulf_susiv2_cv_read(libulf_handle handle, uint16_t cv, int* value);

/**
 * CvWrite
 *
 * \details
 * Write a single Cv using
 * [SUSIV2](https://github.com/ZIMO-Elektronik/ULF_SUSIV2)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \warning
 * Not implemented Yet
 *
 * \param [inout] handle  libulf handle
 * \param [in]    cv      Cv address (zero based, meaning Cv - 1)
 * \param [in]    value   Cv value
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on succes, LIBULF_ERR_ else
 */
libulf_error libulf_susiv2_cv_write(libulf_handle hlib,
                                    uint16_t cv,
                                    uint8_t value,
                                    bool* success);

/**
 * ZPP erase (erase sound flash)
 *
 * \details
 * Erase the entire sound flash using
 * [SUSIV2](https://github.com/ZIMO-Elektronik/ULF_SUSIV2)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_susiv2_zpp_erase(libulf_handle hlib, bool* success);

/**
 * ZPP write (blockwise sound flash write)
 *
 * \details
 * Write sound flash blocks using
 * [SUSIV2](https://github.com/ZIMO-Elektronik/ULF_SUSIV2)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    hzpp    zpp handle
 * \param [in]    index   Block index
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_susiv2_zpp_write(libulf_handle hlib,
                                     zpp_handle hzpp,
                                     uint32_t index,
                                     bool* success);

/**
 * Request features (actually, this just sets the max transfer speed possible)
 *
 * \details
 * Request features using
 * [SUSIV2](https://github.com/ZIMO-Elektronik/ULF_SUSIV2)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_susiv2_features(libulf_handle hlib, bool* success);

/**
 * Exit protocol (plus options)
 *
 * \details
 * Exit protocol using
 * [SUSIV2](https://github.com/ZIMO-Elektronik/ULF_SUSIV2)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib      libulf_handle
 * \param [in]    reboot    Reboot decoder
 * \param [in]    cv8_reset Perform CV8 reset on decoder
 * \param [out]   success   true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_susiv2_exit(libulf_handle hlib,
                                bool reboot,
                                bool cv8_reset,
                                bool* success);

/**
 * ZPP LC DC Query (Checks if the load- / developer- code is valid)
 *
 * \details
 * ZPP LC DC Query using
 * [SUSIV2](https://github.com/ZIMO-Elektronik/ULF_SUSIV2)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_susiv2_zpp_lc_dc_query(libulf_handle hlib,
                                           zpp_handle hzpp,
                                           bool* success);

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

/**
 * Enter Bootloader (via Powercycle)
 *
 * \details
 * Enter Bootloader using
 * [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf_handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_enter_mdu(libulf_handle hlib, bool* success);

/**
 * Enter Bootloader (via OpsMode)
 *
 * \details
 * Enter Bootloader using
 * [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    id      Decoder ID
 * \param [in]    sn      Decoder Serial Number
 * \param [in]    done    True, if this was the last `id-sn` to enter, else
 *                        false
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_enter_dcc_zsu(
  libulf_handle hlib, uint32_t id, uint32_t sn, bool done, bool* success);

/**
 * Enter SoundLoad mode (via OpsMode)
 *
 * \details
 * Enter SoundLoad mode using
 * [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    sn      Decoder Serial Number
 * \param [in]    done    True, if this was the last `sn` to enter, else false
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_enter_dcc_zpp(libulf_handle hlib,
                                          uint32_t sn,
                                          bool done,
                                          bool* success);

/**
 * Ping decoder
 *
 * \details
 * Ping decoder using
 * [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    sn      Decoder Serial Number
 * \param [in]    id      Decoder ID
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_ping(libulf_handle hlib,
                                 uint32_t sn,
                                 uint32_t id,
                                 bool* success);

/**
 * Ping any decoder
 *
 * \details
 * Ping decoders using
 * [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf_handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_ping_all(libulf_handle hlib, bool* success);

/**
 * Configure transfer rate
 *
 * \note
 * Due to problems with earlier Bootloaders, the maximum Transfer Speed for an
 * Update is `Slow` [3]
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib            libulf_handle
 * \param [in]    transfer_rate   Transfer Rate to configure.
 * \param [out]   success         true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_config_transfer_rate(libulf_handle hlib,
                                                 uint8_t transfer_rate,
                                                 bool* success);

/**
 * CV read
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib  libulf handle
 * \param [in]    cv    CV address to read
 * \param [out]   value Read CV value
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error
libulf_mdu_ein_cv_read(libulf_handle hlib, uint16_t cv, int* value);

/**
 * CV write
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    cv      CV address to write
 * \param [in]    value   CV value to write
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_cv_write(libulf_handle hlib,
                                     uint16_t cv,
                                     uint8_t value,
                                     bool* success);

/**
 * Busy query
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_busy(libulf_handle hlib, bool* success);

/**
 * ZPP valid query
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zpp_valid_query(libulf_handle hlib,
                                            zpp_handle hzpp,
                                            bool* success);

/**
 * ZPP LC DC Query
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zpp_lc_dc_query(libulf_handle hlib,
                                            zpp_handle hzpp,
                                            bool* success);

/**
 * ZPP Erase (erases decoder sound flash)
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error
libulf_mdu_ein_zpp_erase(libulf_handle hlib, zpp_handle hzpp, bool* success);

/**
 * ZPP Update
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    hzpp    zpp handle
 * \param [in]    index   Flash block index
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zpp_update(libulf_handle hlib,
                                       zpp_handle hzpp,
                                       uint32_t index,
                                       bool* success);

/**
 * ZPP Update end
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zpp_update_end(libulf_handle hlib,
                                           zpp_handle hzpp,
                                           bool* success);

/**
 * ZPP Exit and Reset
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zpp_exit_reset(libulf_handle hlib, bool* success);

/**
 * ZSU Salsa20 IV
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib            libulf handle
 * \param [in]    hzsu            zsu handle
 * \param [in]    firmware_index  Firmware index
 * \param [out]   success         true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zsu_salsa20_iv(libulf_handle hlib,
                                           zsu_handle hzsu,
                                           size_t firmware_index,
                                           bool* success);

/**
 * ZSU Erase (Erase firmware flash)
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib            libulf handle
 * \param [in]    hzsu            zsu handle
 * \param [in]    firmware_index  Firmware index
 * \param [out]   success         true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zsu_erase(libulf_handle hlib,
                                      zsu_handle hzsu,
                                      size_t firmware_index,
                                      bool* success);

/**
 * ZSU Update
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib            libulf handle
 * \param [in]    hzsu            zsu handle
 * \param [in]    firmware_index  Firmware index
 * \param [in]    index           Firmware block index
 * \param [out]   success         true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zsu_update(libulf_handle hlib,
                                       zsu_handle hzsu,
                                       size_t firmware_index,
                                       uint32_t index,
                                       bool* success);

/**
 * ZSU CRC32 Start
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib            libulf handle
 * \param [in]    hzsu            zsu handle
 * \param [in]    firmware_index  Firmware index
 * \param [out]   success         true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zsu_crc32_start(libulf_handle hlib,
                                            zsu_handle hzsu,
                                            size_t firmware_index,
                                            bool* success);

/**
 * ZSU CRC32 Result
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zsu_crc32_result(libulf_handle hlib, bool* success);

/**
 * ZSU CRC32 Result and Exit
 *
 * \details
 * Uses [MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param [inout] hlib    libulf handle
 * \param [out]   success true, if successful, false else
 *
 * \return libulf_error  LIBULF_OK on success, LIBULF_ERR_ else
 */
libulf_error libulf_mdu_ein_zsu_crc32_result_exit(libulf_handle hlib,
                                                  bool* success);

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

/**
 * Read a ZPP file
 *
 * \warning
 * Any file read, must later be released by the user \see libulf_zpp_release
 *
 * \param [in]  c       ZPP path
 * \param [in]  length  ZPP path length
 *
 * \return zpp_handle Either an handle, or NULL if the file could not be read
 */
zpp_handle libulf_zpp_read(char const* c, size_t length);

/**
 * Release ZPP file
 *
 * \param [in] hzpp  zpp handle
 */
void libulf_zpp_release(zpp_handle hzpp);

/**
 * Get flash block count
 *
 * \param [in] hzpp   zpp handle
 *
 * \return unsigned int Flash block count
 */
unsigned int libulf_zpp_blocks(zpp_handle const hzpp);

/**
 * Get ZPP author
 *
 * \param [in]  hzpp  zpp handle
 *
 * \return char const*  Author string (valid until ZPP is released)
 */
char const* libulf_zpp_author(zpp_handle const hzpp);

/**
 * Get ZPP email
 *
 * \param [in]  hzpp  zpp handle
 *
 * \return char const*  Email string (valid until ZPP is released)
 */
char const* libulf_zpp_email(zpp_handle const hzpp);

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

/**
 * Read a ZSU file
 *
 * \warning
 * Any file read must later be relesed by the user \see libulf_zsu_release
 *
 * \param [in]  c       ZSU file path
 * \param [in]  length  ZSU file path length
 *
 * \return zsu_handle Either a valid handle or NULL if the file cannot be read
 */
zsu_handle libulf_zsu_read(char const* c, size_t length);

/**
 * Release a ZSU file
 *
 * \param [in] hzsu zsu handle
 */
void libulf_zsu_release(zsu_handle hzsu);

/**
 * Get number of firmwares contained in the file
 *
 * \param [in] hzsu zsu handle
 *
 * \return uint32_t Firmware count
 */
uint32_t libulf_zsu_get_firmware_count(zsu_handle const hzsu);

// Ops on firmware

/**
 * Get ID of firmware
 *
 * \details
 * This matches the Decoder ID this firmware is compatible with
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return uint32_t Firmware ID
 */
uint32_t libulf_zsu_get_firmware_id(zsu_handle const hzsu,
                                    size_t const firmware_index);

/**
 * Get Name of firmware
 *
 * \details
 * This matches the name of the Decoder this firmware is compatible with
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return char const*  Firmware name (Valid until ZSU is released)
 */
char const* libulf_zsu_get_firmware_name(zsu_handle const hzsu,
                                         size_t const firmware_index);

/**
 * Get major version of firmware
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return char const*  Firmware major version (Valid until ZSU is released)
 */
char const* libulf_zsu_get_firmware_major_version(zsu_handle const hzsu,
                                                  size_t const firmware_index);

/**
 * Get minor version of firmware
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return char const*  Firmware minor version (Valid until ZSU is released)
 */
char const* libulf_zsu_get_firmware_minor_version(zsu_handle const hzsu,
                                                  size_t const firmware_index);

/**
 * Get Firmware type
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return int  Firmware type
 */
int libulf_zsu_get_firmware_type(zsu_handle const hzsu,
                                 size_t const firmware_index);

/**
 * Get Firmware Block count
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return uint32_t Block count
 */
uint32_t libulf_zsu_get_firmware_block_count(zsu_handle const hzsu,
                                             size_t const firmware_index);

/**
 * Get raw Firmware data
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return uint8_t const* Data (valid until ZSU is released)
 */
uint8_t const* libulf_zsu_get_firmware_data(zsu_handle const hzsu,
                                            size_t const firmware_index);

/**
 * Get raw Firmware data size
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return size_t Data size
 */
size_t libulf_zsu_get_firmware_data_size(zsu_handle const hzsu,
                                         size_t const firmware_index);

#ifdef __cplusplus
}
#endif

#endif
