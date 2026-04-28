#pragma once

#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#include "callback.hpp"

// Opaque poninter as handle
typedef struct bridge_instance* bridge_handle;

/** ---------------------------------------------------
 *  Bridge
 *  ---------------------------------------------------
 */

// Lifetime
bridge_handle bridge_create(void);
void bridge_destroy(bridge_handle handle);

// Callback and result
void bridge_register_cb(bridge_handle handle, bridge_callback cb);
result_t bridge_result(bridge_handle handle);

// Connection Specifics
int bridge_init(bridge_handle handle);
int bridge_open(bridge_handle handle,
                uint16_t vid = 0x1FC9u,
                uint16_t pid = 0x81C1u);
int bridge_openFd(bridge_handle handle, int Fd);
int bridge_config(bridge_handle handle);
int bridge_claim(bridge_handle handle);
int bridge_release(bridge_handle handle);
void bridge_close(bridge_handle handle);

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */

int bridge_com_ping(bridge_handle handle);

#ifdef __cplusplus
}
#endif