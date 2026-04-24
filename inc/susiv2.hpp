#pragma once

#include <cstdint>

extern "C" {

int susiv2_features(bool* result);

int susiv2_cv_read(uint32_t cv, uint8_t* value);

}