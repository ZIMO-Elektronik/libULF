#pragma once

#include <functional>
#include <string>
#include <string_view>
#include "config.hpp"
#include "level.hpp"

namespace internal::log {

using Functor = std::function<void(Level, std::string const&)>;

struct Logger {
  static Logger& get() {
    static Logger instance;
    return instance;
  }

  void setFunctor(Functor functor) { _functor = std::move(functor); }

  void log(Level level, std::string const& message) {
    if constexpr (config::log::log_to_stdout)
      if (level < LogLevel::Warn)
        std::cout << "[" << levelToString(level) << "] " << message
                  << std::endl;

    if constexpr (config::log::log_to_stderr)
      if (level >= LogLevel::Warn)
        std::cerr << "[" << levelToString(level) << "] " << message
                  << std::endl;

    if (_functor) _functor(level, message);
  }

private:
  Logger() = default;

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
}

#define LOG_TRACE(fmt, ...)                                                    \
  MyLib::Logger::getInstance().log(Level::Trace,                               \
                                   std::format(fmt, ##__VA_ARGS__))
#define LOG_DEBUG(fmt, ...)                                                    \
  MyLib::Logger::getInstance().log(Level::Debug,                               \
                                   std::format(fmt, ##__VA_ARGS__))
#define LOG_INFO(fmt, ...)                                                     \
  MyLib::Logger::getInstance().log(Level::Info, std::format(fmt, ##__VA_ARGS__))
#define LOG_WARN(fmt, ...)                                                     \
  MyLib::Logger::getInstance().log(Level::Warn, std::format(fmt, ##__VA_ARGS__))
#define LOG_ERROR(fmt, ...)                                                    \
  MyLib::Logger::getInstance().log(Level::Error,                               \
                                   std::format(fmt, ##__VA_ARGS__))
#define LOG_FATAL(fmt, ...)                                                    \
  MyLib::Logger::getInstance().log(Level::Fatal,                               \
                                   std::format(fmt, ##__VA_ARGS__))

} // namespace internal::log
