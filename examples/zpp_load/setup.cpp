#include "setup.hpp"
#include <cstdlib>

namespace setup {

libklug_handle connect() {
  libklug_handle handle{libklug_create()};
  if (!handle) return handle;

  int rc{0};
  rc = libklug_init(handle);
  if (rc != 0) abort();

  rc = libklug_open(handle, 0x1FC9u, 0x81C1u);
  if (rc != 0) abort();

  return handle;
}

void disconnect(libklug_handle handle) {
  libklug_close(handle);
  libklug_destroy(handle);
}

} // namespace setup
