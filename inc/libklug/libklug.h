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

#include "callback/callback.h"
#include "error/error.h"

// Opaque poninters
typedef struct libklug_instance* libklug_handle;
typedef struct zpp_instance* zpp_handle;
typedef struct zsu_instance* zsu_handle;

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
 * \return int From libusb
 *
 * \todo
 * For now, this directly returns the first libusb error occurred. Maybe we can
 * change this a bit
 */
int libklug_init(libklug_handle handle);

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
 * \return int  From libusb
 *
 * \todo
 * For now, this directly returns the first libusb error occurred. Maybe we can
 * change this a bit
 */
int libklug_open(libklug_handle handle, uint16_t vid, uint16_t pid);

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
 * \return int  From libusb
 *
 * \todo
 * For now, this directly returns the first libusb error occurred. Maybe we can
 * change this a bit
 */
int libklug_openFd(libklug_handle handle, int Fd);

/**
 * Configure device
 *
 * \details
 * Performs internal device setup
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \param handle  libklug handle
 *
 * \return int  From libusb
 *
 * \todo
 * For now, this directly returns the first libusb error occurred. Maybe we can
 * change this a bit
 */
int libklug_config(libklug_handle handle);

/**
 * Claim device
 *
 * \details
 * Claims the configured device
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \note
 * Calling this will detach any previously attached driver. For now, reverting
 * that can be done by simply disconnecting and reconnecting the device.
 *
 * \param handle  libklug handle
 *
 * \return int  From libusb
 *
 * \todo
 * For now, this directly returns the first libusb error occurred. Maybe we can
 * change this a bit
 *
 * \todo
 * Generally, a AIO function should be provided to `connect` a device
 */
int libklug_claim(libklug_handle handle);

/**
 * Release device
 *
 * \details
 * Releases the claimed device
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \note
 * To clarify, this WON'T reattach the detached driver (at least not yet)
 *
 * \param handle  libklug handle
 *
 * \return int  From libusb
 *
 * \todo
 * For now, this directly returns the first libusb error occurred. Maybe we can
 * change this a bit
 */
int libklug_release(libklug_handle handle);

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
 * \todo
 * Generally, a AIO function should be provided to `disconnect` a device
 *
 * \param handle  libklug handle
 */
void libklug_close(libklug_handle handle);

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
 * \retval ok             Success
 * \return libklug_error  Error
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
 * \retval ok             Success
 * \return libklug_error  Error
 */
libklug_error libklug_com_reset(libklug_handle hlib, int* success);

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
 * \retval ok             Success
 * \return libklug_error  Error
 */
libklug_error libklug_com_susiv2(libklug_handle hlib, int* success);

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
 * \retval ok             Success
 * \return libklug_error  Error
 */
libklug_error libklug_com_mdu_ein(libklug_handle hlib, int* success);

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
 * \retval ok             Success
 * \return libklug_error  Error
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
 * \return libklug_error
 */
libklug_error libklug_susiv2_cv_write(libklug_handle hlib,
                                      uint16_t cv,
                                      uint8_t value,
                                      int* success);
libklug_error libklug_susiv2_zpp_erase(libklug_handle hlib, int* success);
libklug_error libklug_susiv2_zpp_write(libklug_handle hlib,
                                       zpp_handle hzpp,
                                       uint32_t index,
                                       int* success);
libklug_error libklug_susiv2_features(libklug_handle hlib, int* success);
libklug_error libklug_susiv2_exit(libklug_handle hlib,
                                  int reboot,
                                  int cv8_reset,
                                  int* success);
libklug_error libklug_susiv2_zpp_lc_dc_query(libklug_handle hlib,
                                             zpp_handle hzpp,
                                             int* success);

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

libklug_error libklug_mdu_ein_enter_mdu(libklug_handle hlib, int* success);
libklug_error libklug_mdu_ein_enter_dcc_zsu(
  libklug_handle hlib, uint32_t id, uint32_t sn, int done, int* success);
libklug_error libklug_mdu_ein_enter_dcc_zpp(libklug_handle hlib,
                                            uint32_t sn,
                                            int done,
                                            int* success);

libklug_error libklug_mdu_ein_ping(libklug_handle hlib,
                                   uint32_t sn,
                                   uint32_t id,
                                   int* success);
libklug_error libklug_mdu_ein_ping_all(libklug_handle hlib, int* success);
libklug_error libklug_mdu_ein_config_transfer_rate(libklug_handle hlib,
                                                   uint8_t transfer_rate,
                                                   int* success);
libklug_error
libklug_mdu_ein_cv_read(libklug_handle hlib, uint16_t cv, uint8_t* value);
libklug_error libklug_mdu_ein_cv_write(libklug_handle hlib,
                                       uint16_t cv,
                                       uint8_t value,
                                       int* success);
libklug_error libklug_mdu_ein_busy(libklug_handle hlib, int* success);

libklug_error libklug_mdu_ein_zpp_valid_query(libklug_handle hlib,
                                              zpp_handle hzpp,
                                              int* success);
libklug_error libklug_mdu_ein_zpp_lc_dc_query(libklug_handle hlib,
                                              zpp_handle hzpp,
                                              int* success);
libklug_error
libklug_mdu_ein_zpp_erase(libklug_handle hlib, zpp_handle hzpp, int* success);
libklug_error libklug_mdu_ein_zpp_update(libklug_handle hlib,
                                         zpp_handle hzpp,
                                         uint32_t index,
                                         int* success);
libklug_error libklug_mdu_ein_zpp_update_end(libklug_handle hlib,
                                             zpp_handle hzpp,
                                             int* success);
libklug_error libklug_mdu_ein_zpp_exit_reset(libklug_handle hlib, int* success);

libklug_error libklug_mdu_ein_zsu_salsa20_iv(libklug_handle hlib,
                                             zsu_handle hzsu,
                                             size_t firmware_index,
                                             int* success);
libklug_error libklug_mdu_ein_zsu_erase(libklug_handle hlib,
                                        zsu_handle hzsu,
                                        size_t firmware_index,
                                        int* success);
libklug_error libklug_mdu_ein_zsu_update(libklug_handle hlib,
                                         zsu_handle hzsu,
                                         size_t firmware_index,
                                         uint32_t index,
                                         int* success);
libklug_error libklug_mdu_ein_zsu_crc32_start(libklug_handle hlib,
                                              zsu_handle hzsu,
                                              size_t firmware_index,
                                              int* success);
libklug_error libklug_mdu_ein_zsu_crc32_result(libklug_handle hlib,
                                               int* success);
libklug_error libklug_mdu_ein_zsu_crc32_result_exit(libklug_handle hlib,
                                                    int* success);

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

zpp_handle libklug_zpp_read(char const* c, size_t length);
void libklug_zpp_release(zpp_handle hzpp);
unsigned int libklug_zpp_blocks(zpp_handle const hzpp);
char const* libklug_zpp_author(zpp_handle const hzpp);
char const* libklug_zpp_email(zpp_handle const hzpp);

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

zsu_handle libklug_zsu_read(char const* c, size_t length);
void libklug_zsu_release(zsu_handle hzsu);

uint32_t libklug_zsu_get_firmware_count(zsu_handle const hzsu);

// Ops on firmware
uint32_t libklug_zsu_get_firmware_id(zsu_handle const hzsu,
                                     size_t const firmware_index);
char const* libklug_zsu_get_firmware_name(zsu_handle const hzsu,
                                          size_t const firmware_index);
char const* libklug_zsu_get_firmware_major_version(zsu_handle const hzsu,
                                                   size_t const firmware_index);
char const* libklug_zsu_get_firmware_minor_version(zsu_handle const hzsu,
                                                   size_t const firmware_index);
int libklug_zsu_get_firmware_type(zsu_handle const hzsu,
                                  size_t const firmware_index);
uint32_t libklug_zsu_get_firmware_block_count(zsu_handle const hzsu,
                                              size_t const firmware_index);
uint8_t const* libklug_zsu_get_firmware_data(zsu_handle const hzsu,
                                             size_t const firmware_index);
size_t libklug_zsu_get_firmware_data_size(zsu_handle const hzsu,
                                          size_t const firmware_index);

#ifdef __cplusplus
}
#endif
