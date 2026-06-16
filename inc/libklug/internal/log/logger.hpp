#pragma once

#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include "config.hpp"
#include "formatter/ranges.hpp"
#include "level.hpp"

namespace internal::log {

using Functor = std::function<void(Level, std::string const&)>;

struct Logger {
  constexpr static Logger& get() {
    static Logger instance;
    return instance;
  }

  constexpr void setFunctor(Functor functor) { _functor = std::move(functor); }
  constexpr void setLevel(Level level) { _level = std::move(level); }

  constexpr void log(Level level, std::string const& message) {
    if (level < _level) return;
    if constexpr (config::log::log_to_stdout)
      if (level < Level::Warning)
        std::cout << "[" << levelToString(level) << "] " << message
                  << std::endl;

    if constexpr (config::log::log_to_stderr)
      if (level >= Level::Warning)
        std::cerr << "[" << levelToString(level) << "] " << message
                  << std::endl;

    if (_functor) _functor(level, message);
  }

private:
  constexpr Logger() = default;

  Functor _functor{nullptr};

  std::string_view levelToString(Level level) {
    using std::operator""sv;
    switch (level) {
      case Level::Trace: return "TRACE"sv;
      case Level::Debug: return "DEBUG"sv;
      case Level::Info: return "INFO"sv;
      case Level::Warning: return "WARN"sv;
      case Level::Error: return "ERROR"sv;
      case Level::Critical: return "CRITICAL"sv;
      default: return "UNKNOWN"sv;
    }
  }

  Level _level{::internal::config::log::default_log_level};
};

} // namespace internal::log

#define LOG_TRACE(fmt, ...)                                                    \
  ::internal::log::Logger::get().log(::internal::log::Level::Trace,            \
                                     std::format(fmt, ##__VA_ARGS__))
#define LOG_DEBUG(fmt, ...)                                                    \
  ::internal::log::Logger::get().log(::internal::log::Level::Debug,            \
                                     std::format(fmt, ##__VA_ARGS__))
#define LOG_INFO(fmt, ...)                                                     \
  ::internal::log::Logger::get().log(::internal::log::Level::Info,             \
                                     std::format(fmt, ##__VA_ARGS__))
#define LOG_WARN(fmt, ...)                                                     \
  ::internal::log::Logger::get().log(::internal::log::Level::Warning,          \
                                     std::format(fmt, ##__VA_ARGS__))
#define LOG_ERROR(fmt, ...)                                                    \
  ::internal::log::Logger::get().log(::internal::log::Level::Error,            \
                                     std::format(fmt, ##__VA_ARGS__))
#define LOG_CRITICAL(fmt, ...)                                                 \
  ::internal::log::Logger::get().log(::internal::log::Level::Critical,         \
                                     std::format(fmt, ##__VA_ARGS__))
