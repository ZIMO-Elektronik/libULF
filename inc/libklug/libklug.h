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

// Opaque poninters as handles
typedef struct libklug_instance* libklug_handle;
typedef struct zpp_instance* zpp_handle;
typedef struct zsu_instance* zsu_handle;
typedef struct firmware_iterator_instance* firmware_iterator_handle;

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

// Lifetime
libklug_handle libklug_create(void);
void libklug_destroy(libklug_handle handle);

// Callback and result
void libklug_register_cb(libklug_handle handle, bridge_callback cb);
result libklug_result(libklug_handle handle);

// Connection Specifics
int libklug_init(libklug_handle handle);
int libklug_open(libklug_handle handle, uint16_t vid, uint16_t pid);
int libklug_openFd(libklug_handle handle, int Fd);
int libklug_config(libklug_handle handle);
int libklug_claim(libklug_handle handle);
int libklug_release(libklug_handle handle);
void libklug_close(libklug_handle handle);

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

int libklug_com_ping(libklug_handle handle);
int libklug_com_reset(libklug_handle handle);
int libklug_com_susiv2(libklug_handle handle);
int libklug_com_mdu_ein(libklug_handle handle);

/** ---------------------------------------------------
 *  Bridge SUSIV2
 *  ---------------------------------------------------
 */

int libklug_susiv2_cv_read(libklug_handle handle, uint16_t cv);
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

zpp_handle
libklug_zpp_read(libklug_handle handle, char const* c, size_t length);
void libklug_zpp_release(libklug_handle handle, zpp_handle zpp);
unsigned int libklug_zpp_blocks(libklug_handle handle, zpp_handle zpp);

/** ---------------------------------------------------
 *  Bridge ZSU
 *  ---------------------------------------------------
 */

zsu_handle
libklug_zsu_read(libklug_handle handle, char const* c, size_t length);
void libklug_zsu_release(libklug_handle handle, zsu_handle zsu);

// Lifetime of iterator
firmware_iterator_handle libklug_zsu_create_firmware_iterator(zsu_handle zsu);
void libklug_zsu_destroy_firmware_iterator(firmware_iterator_handle fw);

// Ops with iterator
int libklug_zsu_firmware_next(firmware_iterator_handle firmware);
int libklug_zsu_firmware_previous(firmware_iterator_handle firmware);
uint32_t libklug_zsu_firmware_id(firmware_iterator_handle firmware);
char const* libklug_zsu_firmware_name(firmware_iterator_handle firmware);
char const*
libklug_zsu_firmware_version_major(firmware_iterator_handle firmware);
char const*
libklug_zsu_firmware_version_minor(firmware_iterator_handle firmware);
int libklug_zsu_firmware_type(firmware_iterator_handle firmware);

uint32_t libklug_zsu_firmware_blocks(libklug_handle handle,
                                     firmware_iterator_handle firmware);

#ifdef __cplusplus
}
#endif
