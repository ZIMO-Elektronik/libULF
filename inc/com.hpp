#pragma once

#include <algorithm>

extern "C" {

int com_ping(char* buffer, std::size_t length);

int com_reset(bool* success);

int com_susiv2(bool* success);
}