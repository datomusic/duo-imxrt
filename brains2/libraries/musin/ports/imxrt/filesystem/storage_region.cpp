#include "musin/filesystem/storage_region.h"

// Defined in the linker script (MIMXRT1011xxxxx_flexspi_nor.ld): the tail of
// the 2 MB flash reserved for the data filesystem, as AHB addresses.
extern "C" uint32_t __flash_fs_start;
extern "C" uint32_t __flash_fs_end;

namespace musin::filesystem {

static constexpr uint32_t FLASH_AHB_BASE = 0x60000000u;

std::optional<StorageRegion> get_storage_region(musin::Logger &logger) {
  const uint32_t start = reinterpret_cast<uint32_t>(&__flash_fs_start);
  const uint32_t end = reinterpret_cast<uint32_t>(&__flash_fs_end);
  if (end <= start || start < FLASH_AHB_BASE) {
    logger.error("StorageRegion: invalid linker-defined flash region");
    return std::nullopt;
  }
  return StorageRegion{start - FLASH_AHB_BASE, end - start};
}

} // namespace musin::filesystem
