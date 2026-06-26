/**
 * Result
 *
 * \file    inc/libklug/result/result.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include "wrapper/cv.hpp"
#include "wrapper/error.hpp"
#include "wrapper/status.hpp"
#include "wrapper/string.hpp"

namespace res {

/**
 * Result
 *
 */
using Result = std::variant<String, Status, Cv, Error>;

} // namespace res
