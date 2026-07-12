#include "settings_storage.h"

#include "etl/array.h"

#include <cstdint>

extern "C" {
#include "fsl_debug_console.h"
}

namespace duo {

namespace {

using musin::settings::Descriptor;

constexpr etl::array<Descriptor, 1> DESCRIPTORS{{
    {setting_id::MIDI_CHANNEL, "midi_channel", 1, 16, 1},
}};

// In-RAM store: values always within their descriptor's range.
class ValueStore : public musin::settings::ValueStore {
public:
  constexpr ValueStore() {
    for (size_t i = 0; i < DESCRIPTORS.size(); ++i) {
      values_[i] = DESCRIPTORS[i].default_value;
    }
  }

  bool set(uint8_t id, uint8_t value) override {
    for (size_t i = 0; i < DESCRIPTORS.size(); ++i) {
      if (DESCRIPTORS[i].id == id) {
        if (value < DESCRIPTORS[i].min || value > DESCRIPTORS[i].max) {
          return false;
        }
        values_[i] = value;
        return true;
      }
    }
    return false;
  }

  uint8_t get(uint8_t id) const override {
    for (size_t i = 0; i < DESCRIPTORS.size(); ++i) {
      if (DESCRIPTORS[i].id == id) {
        return values_[i];
      }
    }
    return 0;
  }

private:
  uint8_t values_[DESCRIPTORS.size()]{};
};

// Logs onto the debug console (LPUART1, shared with DIN MIDI: readable only
// until Serial.begin() reconfigures the UART).
class DebugConsoleLogger : public musin::Logger {
public:
  void log(musin::LogLevel level, etl::string_view message) override {
    if (level < level_) {
      return;
    }
    PRINTF("[fs] %.*s\r\n", static_cast<int>(message.size()), message.data());
  }
  void log(musin::LogLevel level, etl::string_view message,
           std::int32_t value) override {
    if (level < level_) {
      return;
    }
    PRINTF("[fs] %.*s%ld\r\n", static_cast<int>(message.size()),
           message.data(), static_cast<long>(value));
  }
  void log(musin::LogLevel level, etl::string_view message,
           std::uint32_t value) override {
    if (level < level_) {
      return;
    }
    PRINTF("[fs] %.*s%lu\r\n", static_cast<int>(message.size()),
           message.data(), static_cast<unsigned long>(value));
  }
  void log(musin::LogLevel level, etl::string_view message,
           float value) override {
    if (level < level_) {
      return;
    }
    PRINTF("[fs] %.*s%f\r\n", static_cast<int>(message.size()), message.data(),
           static_cast<double>(value));
  }
  void set_level(musin::LogLevel level) override {
    level_ = level;
  }
  musin::LogLevel get_level() const override {
    return level_;
  }

private:
  musin::LogLevel level_ = musin::LogLevel::DEBUG;
};

DebugConsoleLogger g_logger;
musin::filesystem::Filesystem g_filesystem(g_logger);
ValueStore g_value_store;
musin::settings::SettingsManager
    g_settings(etl::span<const Descriptor>(DESCRIPTORS.begin(),
                                           DESCRIPTORS.end()),
               g_value_store, g_logger);

} // namespace

bool storage_init() {
  if (!g_filesystem.init()) {
    return false;
  }
  g_settings.init();
  return true;
}

musin::filesystem::Filesystem &filesystem() {
  return g_filesystem;
}

musin::settings::SettingsManager &settings() {
  return g_settings;
}

} // namespace duo
