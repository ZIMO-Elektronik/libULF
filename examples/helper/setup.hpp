#pragma once

#include <ulf/c/libulf.h>

namespace setup {

static inline libulf_handle connect() {
  libulf_handle handle{libulf_create()};
  if (!handle) return handle;

  int rc{0};
  rc = libulf_init(handle);
  if (rc != 0) abort();

  rc = libulf_open(handle, 0x1FC9u, 0x81C1u);
  if (rc != 0) abort();

  return handle;
}

static inline void disconnect(libulf_handle handle) {
  libulf_close(handle);
  libulf_destroy(handle);
}

} // namespace setup
