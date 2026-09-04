#pragma once

#include <klug/c/libklug.h>

namespace setup {

libklug_handle connect();

void disconnect(libklug_handle handle);

} // namespace setup
