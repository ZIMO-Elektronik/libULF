#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  status,
  cv,
  error,
  libusb_error,
} type;

typedef struct {
  type result_type;
  union {
    bool success;
    int value;
    int error;
    int libusb_error;
  } data;
} result;

// Callback definition
typedef void (*bridge_callback)(result r);

// Callback registration
/// \todo Write registration

#ifdef __cplusplus
}
#endif
