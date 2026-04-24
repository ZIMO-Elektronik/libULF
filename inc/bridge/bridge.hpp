#pragma once

#ifdef __cplusplus
#  include <cstdint>
extern "C" {
#else
#  include <stdint.h>
#endif

// Opaque poninter as handle
typedef struct bridge_instance* bridge_handle;

/** ---------------------------------------------------
 *  Bridge  
 *  ---------------------------------------------------
 */ 

// Lifetime
bridge_handle bridge_create(void);
void bridge_destroy(bridge_handle handle);

// Connection Specifics
int bridge_open(bridge_handle handle,
                uint16_t vid = 0x1FC9u,
                uint16_t pid = 0x81C1u);
int bridge_openFd(bridge_handle handle, int Fd);
int bridge_configure(bridge_handle handle);
int bridge_claim(bridge_handle handle);
int bridge_release(bridge_handle handle);
void bridge_close(bridge_handle handle);

/** ---------------------------------------------------
 *  Bridge COM
 *  ---------------------------------------------------
 */ 

#ifdef __cplusplus
}
#endif