/**
 * Libklug C interface
 *
 * \file    inc/libklug/libklug.h
 * \author  Jonas Gahlert
 * \date    05.05.2026
 */

#pragma once

#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#include "callback.hpp"

// Opaque poninters as handle
typedef struct libklug_instance* libklug_handle;
typedef struct zpp_instance* zpp_handle;

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

// Lifetime
libklug_handle libklug_create(void);
void libklug_destroy(libklug_handle handle);

// Callback and result
void libklug_register_cb(libklug_handle handle, bridge_callback cb);
result_t libklug_result(libklug_handle handle);

// Connection Specifics
int libklug_init(libklug_handle handle);
int libklug_open(libklug_handle handle,
                 uint16_t vid = 0x1FC9u,
                 uint16_t pid = 0x81C1u);
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
int libklug_mdu_ein_cv_read(libklug_handle handle, uint16_t cv);
int libklug_mdu_ein_ping(libklug_handle handle);

/** ---------------------------------------------------
 *  Bridge ZPP
 *  ---------------------------------------------------
 */

zpp_handle
libklug_zpp_read(libklug_handle b_handle, char16_t const* c, size_t length);
void libklug_zpp_release(libklug_handle b_handle, zpp_handle handle);
unsigned int libklug_zpp_blocks(libklug_handle b_handle, zpp_handle handle);

#ifdef __cplusplus
}
#endif
