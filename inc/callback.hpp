#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  status,
  cv,
  string,
  error,
  libusb_error,
} result_type;

typedef struct {
  result_type type;
  union {
    int success;
    int value;
    char const* string;
    int error;
    int libusb_error;
  } data;
} result_t;

// Callback definition
typedef void (*bridge_callback)(result_t r);

// Callback registration
/// \todo Write registration

#ifdef __cplusplus
}
#endif
