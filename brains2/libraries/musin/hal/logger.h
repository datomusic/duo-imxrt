#ifndef LOGGER_H
#define LOGGER_H

#include "etl/string_view.h"
#include <cstdint>

namespace musin {

enum class LogLevel : std::uint8_t {
  DEBUG = 0,
  INFO = 1,
  WARN = 2,
  ERROR = 3,
  NONE = 4
};

class Logger {
public:
  virtual ~Logger() = default;

  virtual void log(LogLevel level, etl::string_view message) = 0;
  virtual void log(LogLevel level, etl::string_view message, std::int32_t value) = 0;
  virtual void log(LogLevel level, etl::string_view message, std::uint32_t value) = 0;
  virtual void log(LogLevel level, etl::string_view message, float value) = 0;

  virtual void set_level(LogLevel level) = 0;
  virtual LogLevel get_level() const = 0;

  void debug(etl::string_view message) {
    log(LogLevel::DEBUG, message);
  }
  void info(etl::string_view message) {
    log(LogLevel::INFO, message);
  }
  void warn(etl::string_view message) {
    log(LogLevel::WARN, message);
  }
  void error(etl::string_view message) {
    log(LogLevel::ERROR, message);
  }

  void debug(etl::string_view message, std::int32_t value) {
    log(LogLevel::DEBUG, message, value);
  }
  void info(etl::string_view message, std::int32_t value) {
    log(LogLevel::INFO, message, value);
  }
  void warn(etl::string_view message, std::int32_t value) {
    log(LogLevel::WARN, message, value);
  }
  void error(etl::string_view message, std::int32_t value) {
    log(LogLevel::ERROR, message, value);
  }

  void debug(etl::string_view message, std::uint32_t value) {
    log(LogLevel::DEBUG, message, value);
  }
  void info(etl::string_view message, std::uint32_t value) {
    log(LogLevel::INFO, message, value);
  }
  void warn(etl::string_view message, std::uint32_t value) {
    log(LogLevel::WARN, message, value);
  }
  void error(etl::string_view message, std::uint32_t value) {
    log(LogLevel::ERROR, message, value);
  }

  void debug(etl::string_view message, float value) {
    log(LogLevel::DEBUG, message, value);
  }
  void info(etl::string_view message, float value) {
    log(LogLevel::INFO, message, value);
  }
  void warn(etl::string_view message, float value) {
    log(LogLevel::WARN, message, value);
  }
  void error(etl::string_view message, float value) {
    log(LogLevel::ERROR, message, value);
  }
};

} // namespace musin
#endif
