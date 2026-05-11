#pragma once

#include <cstdio>
#include <array>
#include <functional>
#include <string_view>
#include <mutex>

#include "utility.h"

namespace logger {

#ifndef LOGGER_DEFAULT_ENABLED
  #define LOGGER_DEFAULT_ENABLED 1
#endif
#ifndef LOGGER_MIN_LOG_LEVEL
  #define LOGGER_MIN_LOG_LEVEL TRACE
#endif

constexpr const int enabled_default_output = LOGGER_DEFAULT_ENABLED;

enum class LogLevel : uint8_t {
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  CRITICAL,
  DISABLE_LOGGING
};
inline constexpr LogLevel minimum_level = LogLevel::LOGGER_MIN_LOG_LEVEL;
constexpr const std::array loglevel_to_sv_lookup {"trace", "debug", "info", "warning", "error", "critical"};

extern std::function<void(const std::string_view)> logger_callback;
inline void set_logger_callback(std::function<void(const std::string_view)> callback) {
  logger_callback = callback;
}

thread_local static char buffer[512] {};
extern std::mutex log_write_mutex;
template <typename... Args> inline void log(const LogLevel level, const char* fmt, Args&&... args) {
  size_t length = 0;
  length += snprintf(buffer, sizeof(buffer), "[%*s] ", 8, loglevel_to_sv_lookup.at(to_integral(level)));
  if constexpr (sizeof...(Args)) length += snprintf(buffer + length, sizeof(buffer) - length, fmt, std::forward<Args>(args)...);
  else length += snprintf(buffer + length, sizeof(buffer) - length, "%s", fmt);

  {
    std::scoped_lock log_write_lock(log_write_mutex);
    if (logger_callback) {
      logger_callback(buffer);
    }
    if constexpr (enabled_default_output) {
      if (level > LogLevel::WARN) fprintf(stderr, "%.*s\n", static_cast<int>(length), buffer);
      fprintf(stdout, "%.*s\n", static_cast<int>(length), buffer);
    }
  }
}

template <typename... Args> inline void trace(const char* fmt, Args&&... args) {
  if constexpr (minimum_level <= LogLevel::TRACE) {
    log(LogLevel::TRACE, fmt, std::forward<Args>(args)...);
  }
}
template <typename... Args> inline void debug(const char* fmt, Args&&... args) {
  if constexpr (minimum_level <= LogLevel::DEBUG) {
    log(LogLevel::DEBUG, fmt, std::forward<Args>(args)...);
  }
}
template <typename... Args> inline void info(const char* fmt, Args&&... args) {
  if constexpr (minimum_level <= LogLevel::INFO) {
    log(LogLevel::INFO, fmt, std::forward<Args>(args)...);
  }
}
template <typename... Args> inline void warning(const char* fmt, Args&&... args) {
  if constexpr (minimum_level <= LogLevel::WARN) {
    log(LogLevel::WARN, fmt, std::forward<Args>(args)...);
  }
}
template <typename... Args> inline void error(const char* fmt, Args&&... args) {
  if constexpr (minimum_level <= LogLevel::ERROR) {
    log(LogLevel::ERROR, fmt, std::forward<Args>(args)...);
  }
}
template <typename... Args> inline void critical(const char* fmt, Args&&... args) {
  if constexpr (minimum_level <= LogLevel::CRITICAL) {
    log(LogLevel::CRITICAL, fmt, std::forward<Args>(args)...);
  }
}

}
