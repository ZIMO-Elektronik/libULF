/**
 * Copyright (C) 2026 ZIMO Elektronik
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 *
 *
 *
 *
 *
 * Log level
 *
 * \file    src/log/level.hpp
 * \author  Jonas Gahlert
 * \date    04.09.2026
 */

#pragma once

namespace internal::log {

/**
 * Log Level
 *
 */
enum class Level : int {
  Trace = 0,    ///< Detailed diagnostic, can contain e.g. byte-dumps ...
  Debug = 1,    ///< Diagnostic e.g. API call trace, paths, ...
  Info = 2,     ///< "Normal" events
  Warning = 3,  ///< Unexpected events, e.g. slow speed
  Error = 4,    ///< Concrete error
  Critical = 5, ///< Critical error, may result in a crash
};

} // namespace internal::log
