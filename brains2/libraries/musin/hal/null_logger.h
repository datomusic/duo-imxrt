#ifndef NULL_LOGGER_H
#define NULL_LOGGER_H

#include "logger.h"

namespace musin {

class NullLogger : public Logger {
public:
  void log(LogLevel, etl::string_view) override {
  }
  void log(LogLevel, etl::string_view, std::int32_t) override {
  }
  void log(LogLevel, etl::string_view, std::uint32_t) override {
  }
  void log(LogLevel, etl::string_view, float) override {
  }
  void set_level(LogLevel) override {
  }
  LogLevel get_level() const override {
    return LogLevel::NONE;
  }
};

} // namespace musin
#endif
