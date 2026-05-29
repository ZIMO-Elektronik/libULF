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
