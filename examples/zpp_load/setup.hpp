#pragma once

#include <ulf/c/libulf.h>

namespace setup {

libulf_handle connect();

void disconnect(libulf_handle handle);

} // namespace setup
