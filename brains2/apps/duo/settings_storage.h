#ifndef DUO_SETTINGS_STORAGE_H
#define DUO_SETTINGS_STORAGE_H

// Arduino's Common.h defines min/max as macros, which breaks the ETL
// headers the musin settings layer uses. Nothing in this firmware uses the
// macro forms.
#undef min
#undef max

#include "musin/filesystem/filesystem.h"
#include "musin/settings/settings_manager.h"

namespace duo {

/**
 * @brief Setting ids, stable on the wire and on flash. Never reuse a value
 * for a different setting once released.
 */
namespace setting_id {
inline constexpr uint8_t MIDI_CHANNEL = 0x01;
} // namespace setting_id

/**
 * @brief Mounts the littlefs data filesystem (formatting on first boot) and
 * loads persisted settings. Call once, early in main(), before audio runs:
 * a first-boot format erases flash with interrupts disabled per sector.
 *
 * @return false if the filesystem could not be mounted; settings then keep
 * their compile-time defaults and set() calls will not persist.
 */
bool storage_init();

/** @brief The mounted filesystem. Valid regardless of mount success. */
musin::filesystem::Filesystem &filesystem();

/** @brief Device settings, loaded from flash. Valid after storage_init(). */
musin::settings::SettingsManager &settings();

} // namespace duo

#endif
