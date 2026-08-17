#include "settings_storage.h"

#include "etl/array.h"
#include "musin/hal/null_logger.h"

#include <cstdint>

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

// The debug console shares LPUART1 with DIN MIDI, so nothing may be logged:
// a connected MIDI device would receive the bytes as MIDI data.
musin::NullLogger g_logger;
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
