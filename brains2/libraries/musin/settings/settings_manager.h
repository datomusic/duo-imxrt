#ifndef MUSIN_SETTINGS_SETTINGS_MANAGER_H
#define MUSIN_SETTINGS_SETTINGS_MANAGER_H

#include "etl/span.h"
#include "etl/string_view.h"
#include "musin/hal/logger.h"
#include <cstdint>

namespace musin::settings {

/**
 * @brief Describes one setting: wire id, short name, valid range and default.
 *
 * The name doubles as the filename under /settings/ on the device
 * filesystem, so keep it short, lowercase and filesystem-safe. Ids must stay
 * 7-bit safe (0x01-0x7F) and must never be reused for a different setting
 * once released.
 */
struct Descriptor {
  uint8_t id;
  etl::string_view name;
  uint8_t min;
  uint8_t max;
  uint8_t default_value;
};

/**
 * @brief The in-RAM store SettingsManager reads and writes through.
 *
 * Implementations validate id and range; set() returns false for values it
 * rejects.
 */
class ValueStore {
public:
  virtual bool set(uint8_t id, uint8_t value) = 0;
  [[nodiscard]] virtual uint8_t get(uint8_t id) const = 0;

protected:
  ~ValueStore() = default;
};

/**
 * @brief Persists settings as one file per setting under /settings/.
 *
 * The filename is the setting's short name and the file content is the raw
 * value byte. A missing or unreadable file means the compile-time default
 * applies, so no migration or versioning of the store is needed: unknown
 * files are ignored and absent files fall back to defaults.
 *
 * Each product supplies its own descriptor table; file access goes through
 * plain POSIX stdio, which every musin filesystem port must wire up.
 */
class SettingsManager {
public:
  SettingsManager(etl::span<const Descriptor> descriptors, ValueStore &store,
                  musin::Logger &logger);

  /**
   * @brief Loads all known settings from the filesystem.
   * Call once after the filesystem is mounted.
   */
  void init();

  [[nodiscard]] uint8_t get(uint8_t id) const;

  /**
   * @brief Validates, applies and persists a setting value.
   * @return false for unknown ids, out-of-range values, or write failures.
   */
  bool set(uint8_t id, uint8_t value);

private:
  [[nodiscard]] const Descriptor *find_descriptor(uint8_t id) const;
  void load_one(const Descriptor &descriptor);
  bool persist_one(const Descriptor &descriptor, uint8_t value);

  etl::span<const Descriptor> descriptors_;
  ValueStore &store_;
  musin::Logger &logger_;
};

} // namespace musin::settings

#endif // MUSIN_SETTINGS_SETTINGS_MANAGER_H
