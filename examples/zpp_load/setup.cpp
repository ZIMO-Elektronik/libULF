#include "setup.hpp"
#include <cstdlib>

namespace setup {

bridge_handle connect() {
  bridge_handle handle{bridge_create()};
  if (!handle) return handle;

  int rc{0};
  rc = bridge_init(handle);
  if (rc != 0) abort();

  rc = bridge_open(handle);
  if (rc != 0) abort();

  rc = bridge_config(handle);
  if (rc != 0) abort();

  rc = bridge_claim(handle);
  if (rc != 0) abort();

  return handle;
}

void disconnect(bridge_handle handle) {
  bridge_release(handle);
  bridge_close(handle);
  bridge_destroy(handle);
}

} // namespace setup
