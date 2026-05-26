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

// Opaque poninters
typedef struct libklug_instance* libklug_handle;
typedef struct zpp_instance* zpp_handle;
typedef struct zsu_instance* zsu_handle;
typedef struct firmware_iterator_instance* firmware_iterator_handle;

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

// --- Callback and result --- //

/**
 * Register a callback
 *
 * \details
 * Provided callback will be registered on the given libklug object.
 *
 * \note
 * The callback should be something that can be called async without crashing.
 * As an example, providing a callback on Android without precautions will
 * result in a crash upon call.
 *
 * \note
 * This will automatically deregister any previously registered cb
 *
 * \warning
 * It is illegal to pass NULL, Passing an invalid handle or callback results in
 * UB and should be avoided
 *
 * \todo
 * Add some form of manual deregister
 *
 * \param handle  libklug handle
 * \param cb      Callback
 */
void libklug_register_cb(libklug_handle handle, bridge_callback cb);

/**
 * Get result of last operation
 *
 * \details
 * Since most operations communicating with an ULF_COM device are async, the
 * result can be polled here.
 *
 * \note
 * This will hang UNTIL a result is available.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \warning
 * Calling this without a pending result will result in an infinite wait
 *
 * \todo
 * Modify backend so using this wont result in an infinite wait...
 *
 * \todo
 * Add more methods of polling
 *
 * \param handle  libklug handle
 *
 * \return result Result of last operation
 */
result libklug_result(libklug_handle handle);

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
 * Ping device (async)
 *
 * \details
 * Performs an [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) Ping.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \result
 * This will produce a result_type::String on success
 *
 * \param handle  libklug handle
 *
 * \return int
 * \retval LIBKLUG_TRUE   Started
 * \retval LIBKLUG_FALSE  Busy
 */
int libklug_com_ping(libklug_handle handle);

/**
 * Reset device (async)
 *
 * \details
 * Performs an [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) Reset.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \result
 * This will produce a result_type::Status on success
 *
 * \param handle  libklug handle
 *
 * \return int
 * \retval LIBKLUG_TRUE   Started
 * \retval LIBKLUG_FALSE  Busy
 */
int libklug_com_reset(libklug_handle handle);

/**
 * Enter SUSIV2 Mode (async)
 *
 * \details
 * Performs an [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) SUSIV2.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \result
 * This will produce a result_type::Status on success
 *
 * \param handle  libklug handle
 *
 * \return int
 * \retval LIBKLUG_TRUE   Started
 * \retval LIBKLUG_FALSE  Busy
 */
int libklug_com_susiv2(libklug_handle handle);

/**
 * Enter MDU_EIN Mode (async)
 *
 * \details
 * Performs an [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) MDU_EIN.
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \result
 * This will produce a result_type::Status on success
 *
 * \param handle  libklug handle
 *
 * \return int
 * \retval LIBKLUG_TRUE   Started
 * \retval LIBKLUG_FALSE  Busy
 */
int libklug_com_mdu_ein(libklug_handle handle);

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

/**
 * CvRead (async)
 *
 * \details
 * Read a single Cv using
 * [SUSIV2](https://github.com/ZIMO-Elektronik/ULF_SUSIV2)
 *
 * \warning
 * It is illegal to pass NULL. Passing an invalid handle results in UB and
 * should be avoided
 *
 * \result
 * This will produce a result_type::Cv on success
 *
 * \param handle  libklug handle
 * \param cv      Cv address (zero-based, meaning Cv - 1)
 *
 * \return int
 * \retval LIBKLUG_TRUE   Started
 * \retval LIBKLUG_FALSE  Busy
 */
int libklug_susiv2_cv_read(libklug_handle handle, uint16_t cv);

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
 * \result
 * This will produce a result_type::Status on success
 *
 * \param handle  libklug handle
 * \param cv      Cv address (zero based, meaning Cv - 1)
 * \param value   Cv value
 *
 * \return int
 * \retval LIBKLUG_TRUE   Started
 * \retval LIBKLUG_FALSE  Busy
 */
int libklug_susiv2_cv_write(libklug_handle handle, uint16_t cv, uint8_t value);
int libklug_susiv2_zpp_erase(libklug_handle handle);
int libklug_susiv2_zpp_write(libklug_handle handle,
                             zpp_handle file_handle,
                             uint32_t index);
int libklug_susiv2_features(libklug_handle handle);
int libklug_susiv2_exit(libklug_handle handle, int reboot, int cv8_reset);
int libklug_susiv2_zpp_lc_dc_query(libklug_handle handle,
                                   zpp_handle file_handle);

/** ---------------------------------------------------
 *  Bridge MDU_EIN
 *  ---------------------------------------------------
 */

int libklug_mdu_ein_enter_mdu(libklug_handle handle);
int libklug_mdu_ein_enter_dcc_zsu(libklug_handle handle);
int libklug_mdu_ein_enter_dcc_zpp(libklug_handle handle);

int libklug_mdu_ein_ping(libklug_handle handle, uint32_t sn, uint32_t id);
int libklug_mdu_ein_ping_all(libklug_handle handle);
int libklug_mdu_ein_config_transfer_rate(libklug_handle handle,
                                         uint8_t transfer_rate);
int libklug_mdu_ein_cv_read(libklug_handle handle, uint16_t cv);
int libklug_mdu_ein_cv_write(libklug_handle handle, uint16_t cv, uint8_t value);
int libklug_mdu_ein_busy(libklug_handle handle);

int libklug_mdu_ein_zpp_valid_query(libklug_handle handle, zpp_handle zpp);
int libklug_mdu_ein_zpp_lc_dc_query(libklug_handle handle, zpp_handle zpp);
int libklug_mdu_ein_zpp_erase(libklug_handle handle, zpp_handle zpp);
int libklug_mdu_ein_zpp_update(libklug_handle handle,
                               zpp_handle zpp,
                               uint32_t index);
int libklug_mdu_ein_zpp_update_end(libklug_handle handle, zpp_handle zpp);
int libklug_mdu_ein_zpp_exit_reset(libklug_handle handle);

int libklug_mdu_ein_zsu_salsa20_iv(libklug_handle handle,
                                   firmware_iterator_handle firmware);
int libklug_mdu_ein_zsu_erase(libklug_handle handle,
                              firmware_iterator_handle firmware);
int libklug_mdu_ein_zsu_update(libklug_handle handle,
                               firmware_iterator_handle firmware,
                               uint32_t index);
int libklug_mdu_ein_zsu_crc32_start(libklug_handle handle,
                                    firmware_iterator_handle firmware);
int libklug_mdu_ein_zsu_crc32_result(libklug_handle handle);
int libklug_mdu_ein_zsu_crc32_result_exit(libklug_handle handle);

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

zpp_handle libklug_zpp_read(char const* c, size_t length);
void libklug_zpp_release(zpp_handle zpp);
unsigned int libklug_zpp_blocks(zpp_handle zpp);

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

zsu_handle libklug_zsu_read(char const* c, size_t length);
void libklug_zsu_release(zsu_handle zsu);

// Lifetime of iterator
firmware_iterator_handle
libklug_zsu_firmware_iterator_create_begin(zsu_handle zsu);
firmware_iterator_handle
libklug_zsu_firmware_iterator_create_end(zsu_handle zsu);
void libklug_zsu_destroy_firmware_iterator(firmware_iterator_handle fw);

// Ops with iterator
int libklug_zsu_firmware_iterator_next(firmware_iterator_handle firmware);
int libklug_zsu_firmware_iterator_previous(firmware_iterator_handle firmware);
int libklug_zsu_firmware_iterator_equals(firmware_iterator_handle lhs,
                                         firmware_iterator_handle rhs);

// Ops on firmware
uint32_t
libklug_zsu_firmware_iterator_get_id(firmware_iterator_handle firmware);
char const*
libklug_zsu_firmware_iterator_get_name(firmware_iterator_handle firmware);
char const* libklug_zsu_firmware_iterator_get_version_major(
  firmware_iterator_handle firmware);
char const* libklug_zsu_firmware_iterator_get_version_minor(
  firmware_iterator_handle firmware);
int libklug_zsu_firmware_iterator_get_type(firmware_iterator_handle firmware);
uint32_t
libklug_zsu_firmware_iterator_get_blocks(firmware_iterator_handle firmware);

#ifdef __cplusplus
}
#endif
