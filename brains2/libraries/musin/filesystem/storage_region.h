#ifndef MUSIN_FILESYSTEM_STORAGE_REGION_H_
#define MUSIN_FILESYSTEM_STORAGE_REGION_H_

#include "musin/hal/logger.h"
#include <cstdint>
#include <optional>

namespace musin::filesystem {

/**
 * @brief The flash region that holds the data filesystem.
 *
 * Offset and size are in bytes relative to the start of flash and must be
 * erase-sector aligned.
 */
struct StorageRegion {
  uint32_t offset;
  uint32_t size;
};

/**
 * @brief Returns the flash region reserved for the data filesystem.
 *
 * Implemented by each port: the Pico port discovers the region via the
 * RP2350 bootrom partition table, other ports may return a fixed
 * linker-defined offset and size.
 *
 * @return The storage region, or std::nullopt if none is available.
 */
std::optional<StorageRegion> get_storage_region(musin::Logger &logger);

} // namespace musin::filesystem

#endif // MUSIN_FILESYSTEM_STORAGE_REGION_H_
