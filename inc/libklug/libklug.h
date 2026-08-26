/**
 * Libklug C interface
 *
 * \file    inc/libklug/libklug.h
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#ifdef __cplusplus
#  include <cstddef>
#  include <cstdint>
extern "C" {
#else
#  include <stddef.h>
#  include <stdint.h>
#endif

#include "error/error.h"

// Opaque poninters
typedef struct libklug_instance* libklug_handle;
typedef struct zpp_instance* zpp_handle;
typedef struct zsu_instance* zsu_handle;

typedef enum libklug_bool_t {
  LIBKLUG_TRUE = 0,
  LIBKLUG_FALSE = 1,
} libklug_bool;

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

// --- Lifetime --- //

/**
 * Create a libklug object
 *
 * \details
 * Allocated libklug object is made available using an opaque pointer
 *
 * \warning
 * A created libklug object MUST be destroyed using `libklug_destroy` to avoid a
 * memory leak. The user is responsible for keeping the handle alive until then.
 *
 * \return libklug_handle
 */
libklug_handle libklug_create();

/**
 * Destroy a libklug object
 *
 * \details
 * Deallocates a previously created libklug object using the given handle.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param handle libklug handle
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
void libklug_destroy(libklug_handle handle);

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
 * \param handle libklug handle
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error libklug_init(libklug_handle handle);

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
 * \param handle  libklug handle
 * \param vid     Device VID
 * \param pid     Device PID
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error libklug_open(libklug_handle handle, uint16_t vid, uint16_t pid);

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
 * \param handle  libklug handle
 * \param Fd      File descriptor
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error libklug_openFd(libklug_handle handle, int Fd);

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
 * \param handle  libklug handle
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error libklug_close(libklug_handle handle);

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
 * \param [inout] handle  libklug handle
 * \param [out]   buf     The buffer to copy the result string to
 * \param [inout] len     The size of the buffer. On success, `len` will contain
 *                        the actual byte count copied
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error libklug_com_ping(libklug_handle hlib, char* buf, size_t* len);

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
 * \param [inout] handle  libklug handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error libklug_com_reset(libklug_handle hlib, libklug_bool* success);

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
 * \param [inout] handle  libklug handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error libklug_com_susiv2(libklug_handle hlib, libklug_bool* success);

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
 * \param [inout] handle  libklug handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error libklug_com_mdu_ein(libklug_handle hlib, libklug_bool* success);

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
 * \param [inout] handle  libklug handle
 * \param [in]    cv      Cv address (zero-based, meaning Cv - 1)
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error
libklug_susiv2_cv_read(libklug_handle handle, uint16_t cv, uint8_t* value);

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
 * \param [inout] handle  libklug handle
 * \param [in]    cv      Cv address (zero based, meaning Cv - 1)
 * \param [in]    value   Cv value
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on succes, LIBKLUG_ERR_ else
 */
libklug_error libklug_susiv2_cv_write(libklug_handle hlib,
                                      uint16_t cv,
                                      uint8_t value,
                                      libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_susiv2_zpp_erase(libklug_handle hlib,
                                       libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    hzpp    zpp handle
 * \param [in]    index   Block index
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_susiv2_zpp_write(libklug_handle hlib,
                                       zpp_handle hzpp,
                                       uint32_t index,
                                       libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_susiv2_features(libklug_handle hlib,
                                      libklug_bool* success);

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
 * \param [inout] hlib      libklug_handle
 * \param [in]    reboot    Reboot decoder
 * \param [in]    cv8_reset Perform CV8 reset on decoder
 * \param [out]   success   Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_susiv2_exit(libklug_handle hlib,
                                  libklug_bool reboot,
                                  libklug_bool cv8_reset,
                                  libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_susiv2_zpp_lc_dc_query(libklug_handle hlib,
                                             zpp_handle hzpp,
                                             libklug_bool* success);

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
 * \param [inout] hlib    libklug_handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_enter_mdu(libklug_handle hlib,
                                        libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    id      Decoder ID
 * \param [in]    sn      Decoder Serial Number
 * \param [in]    done    True, if this was the last `id-sn` to enter, else
 *                        false
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_enter_dcc_zsu(libklug_handle hlib,
                                            uint32_t id,
                                            uint32_t sn,
                                            libklug_bool done,
                                            libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    sn      Decoder Serial Number
 * \param [in]    done    True, if this was the last `sn` to enter, else false
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_enter_dcc_zpp(libklug_handle hlib,
                                            uint32_t sn,
                                            libklug_bool done,
                                            libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    sn      Decoder Serial Number
 * \param [in]    id      Decoder ID
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_ping(libklug_handle hlib,
                                   uint32_t sn,
                                   uint32_t id,
                                   libklug_bool* success);

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
 * \param [inout] hlib    libklug_handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_ping_all(libklug_handle hlib,
                                       libklug_bool* success);

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
 * \param [inout] hlib            libklug_handle
 * \param [in]    transfer_rate   Transfer Rate to configure.
 * \param [out]   success         Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_config_transfer_rate(libklug_handle hlib,
                                                   uint8_t transfer_rate,
                                                   libklug_bool* success);

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
 * \param [inout] hlib  libklug handle
 * \param [in]    cv    CV address to read
 * \param [out]   value Read CV value
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error
libklug_mdu_ein_cv_read(libklug_handle hlib, uint16_t cv, uint8_t* value);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    cv      CV address to write
 * \param [in]    value   CV value to write
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_cv_write(libklug_handle hlib,
                                       uint16_t cv,
                                       uint8_t value,
                                       libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_busy(libklug_handle hlib, libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zpp_valid_query(libklug_handle hlib,
                                              zpp_handle hzpp,
                                              libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zpp_lc_dc_query(libklug_handle hlib,
                                              zpp_handle hzpp,
                                              libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zpp_erase(libklug_handle hlib,
                                        zpp_handle hzpp,
                                        libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    hzpp    zpp handle
 * \param [in]    index   Flash block index
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zpp_update(libklug_handle hlib,
                                         zpp_handle hzpp,
                                         uint32_t index,
                                         libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [in]    hzpp    zpp handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zpp_update_end(libklug_handle hlib,
                                             zpp_handle hzpp,
                                             libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zpp_exit_reset(libklug_handle hlib,
                                             libklug_bool* success);

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
 * \param [inout] hlib            libklug handle
 * \param [in]    hzsu            zsu handle
 * \param [in]    firmware_index  Firmware index
 * \param [out]   success         Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zsu_salsa20_iv(libklug_handle hlib,
                                             zsu_handle hzsu,
                                             size_t firmware_index,
                                             libklug_bool* success);

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
 * \param [inout] hlib            libklug handle
 * \param [in]    hzsu            zsu handle
 * \param [in]    firmware_index  Firmware index
 * \param [out]   success         Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zsu_erase(libklug_handle hlib,
                                        zsu_handle hzsu,
                                        size_t firmware_index,
                                        libklug_bool* success);

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
 * \param [inout] hlib            libklug handle
 * \param [in]    hzsu            zsu handle
 * \param [in]    firmware_index  Firmware index
 * \param [in]    index           Firmware block index
 * \param [out]   success         Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zsu_update(libklug_handle hlib,
                                         zsu_handle hzsu,
                                         size_t firmware_index,
                                         uint32_t index,
                                         libklug_bool* success);

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
 * \param [inout] hlib            libklug handle
 * \param [in]    hzsu            zsu handle
 * \param [in]    firmware_index  Firmware index
 * \param [out]   success         Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zsu_crc32_start(libklug_handle hlib,
                                              zsu_handle hzsu,
                                              size_t firmware_index,
                                              libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zsu_crc32_result(libklug_handle hlib,
                                               libklug_bool* success);

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
 * \param [inout] hlib    libklug handle
 * \param [out]   success Contains either LIBKLUG_TRUE or LIBKLUG_FALSE
 *
 * \return libklug_error  LIBKLUG_OK on success, LIBKLUG_ERR_ else
 */
libklug_error libklug_mdu_ein_zsu_crc32_result_exit(libklug_handle hlib,
                                                    libklug_bool* success);

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

/**
 * Read a ZPP file
 *
 * \warning
 * Any file read, must later be released by the user \see libklug_zpp_release
 *
 * \param [in]  c       ZPP path
 * \param [in]  length  ZPP path length
 *
 * \return zpp_handle Either an handle, or NULL if the file could not be read
 */
zpp_handle libklug_zpp_read(char const* c, size_t length);

/**
 * Release ZPP file
 *
 * \param [in] hzpp  zpp handle
 */
void libklug_zpp_release(zpp_handle hzpp);

/**
 * Get flash block count
 *
 * \param [in] hzpp   zpp handle
 *
 * \return unsigned int Flash block count
 */
unsigned int libklug_zpp_blocks(zpp_handle const hzpp);

/**
 * Get ZPP author
 *
 * \param [in]  hzpp  zpp handle
 *
 * \return char const*  Author string (valid until ZPP is released)
 */
char const* libklug_zpp_author(zpp_handle const hzpp);

/**
 * Get ZPP email
 *
 * \param [in]  hzpp  zpp handle
 *
 * \return char const*  Email string (valid until ZPP is released)
 */
char const* libklug_zpp_email(zpp_handle const hzpp);

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

/**
 * Read a ZSU file
 *
 * \warning
 * Any file read must later be relesed by the user \see libklug_zsu_release
 *
 * \param [in]  c       ZSU file path
 * \param [in]  length  ZSU file path length
 *
 * \return zsu_handle Either a valid handle or NULL if the file cannot be read
 */
zsu_handle libklug_zsu_read(char const* c, size_t length);

/**
 * Release a ZSU file
 *
 * \param [in] hzsu zsu handle
 */
void libklug_zsu_release(zsu_handle hzsu);

/**
 * Get number of firmwares contained in the file
 *
 * \param [in] hzsu zsu handle
 *
 * \return uint32_t Firmware count
 */
uint32_t libklug_zsu_get_firmware_count(zsu_handle const hzsu);

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
uint32_t libklug_zsu_get_firmware_id(zsu_handle const hzsu,
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
char const* libklug_zsu_get_firmware_name(zsu_handle const hzsu,
                                          size_t const firmware_index);

/**
 * Get major version of firmware
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return char const*  Firmware major version (Valid until ZSU is released)
 */
char const* libklug_zsu_get_firmware_major_version(zsu_handle const hzsu,
                                                   size_t const firmware_index);

/**
 * Get minor version of firmware
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return char const*  Firmware minor version (Valid until ZSU is released)
 */
char const* libklug_zsu_get_firmware_minor_version(zsu_handle const hzsu,
                                                   size_t const firmware_index);

/**
 * Get Firmware type
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return int  Firmware type
 */
int libklug_zsu_get_firmware_type(zsu_handle const hzsu,
                                  size_t const firmware_index);

/**
 * Get Firmware Block count
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return uint32_t Block count
 */
uint32_t libklug_zsu_get_firmware_block_count(zsu_handle const hzsu,
                                              size_t const firmware_index);

/**
 * Get raw Firmware data
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return uint8_t const* Data (valid until ZSU is released)
 */
uint8_t const* libklug_zsu_get_firmware_data(zsu_handle const hzsu,
                                             size_t const firmware_index);

/**
 * Get raw Firmware data size
 *
 * \param [in] hzsu           zsu handle
 * \param [in] firmware_index Index of firmware
 *
 * \return size_t Data size
 */
size_t libklug_zsu_get_firmware_data_size(zsu_handle const hzsu,
                                          size_t const firmware_index);

#ifdef __cplusplus
}
#endif
