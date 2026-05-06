/**
 * Libklug C++ interface
 *
 * \file    inc/libklug/libklug.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include "internal/bridge/bridge.hpp"

/// \todo This namespace should be top-level within the project
namespace libklug {

/**
 * LibKLUG class alias
 *
 */
using LibKLUG = bridge::Bridge;

} // namespace libklug
