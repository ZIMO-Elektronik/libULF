#pragma once

#include <bridge/bridge.hpp>

namespace setup {

bridge_handle connect();

void disconnect(bridge_handle handle);

} // namespace setup
