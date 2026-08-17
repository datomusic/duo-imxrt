#ifndef FILESYSTEM_H_A1PWKQIM
#define FILESYSTEM_H_A1PWKQIM

#include "musin/hal/logger.h"
#include <cstdint>

// Forward declarations for filesystem types
typedef struct filesystem filesystem_t;
typedef struct blockdevice blockdevice_t;

namespace musin::filesystem {

struct StorageInfo {
  uint32_t total_bytes;
  uint32_t free_bytes;
};

/**
 * @brief Platform-neutral filesystem facade.
 *
 * Each port supplies the implementation (musin/ports/<platform>/filesystem/)
 * backed by littlefs on that platform's flash. Ports must also wire the
 * mounted filesystem into newlib/stdio so consumers can use plain
 * fopen/fread/mkdir — POSIX stdio is the official seam for portable code.
 */
class Filesystem {
public:
  explicit Filesystem(musin::Logger &logger);

  /**
   * @brief Initializes the filesystem.
   *
   * This function attempts to mount the existing filesystem. If mounting fails,
   * it will automatically attempt to format and then mount.
   *
   * @return true if the filesystem is successfully initialized (mounted), false
   * otherwise.
   */
  bool init();

  /**
   * @brief Formats the filesystem.
   *
   * This function will format the filesystem and then mount it. Use this when
   * you explicitly want to erase all data and start fresh.
   *
   * @return true if the filesystem is successfully formatted and mounted, false
   * otherwise.
   */
  bool format();

  /**
   * @brief Lists all files and directories at the given path.
   *
   * @param path The directory path to list (e.g., "/").
   */
  void list_files(const char *path);

  /**
   * @brief Gets the total and free space of the filesystem.
   *
   * @return A StorageInfo struct containing the total and free space in bytes.
   */
  StorageInfo get_storage_info();

private:
  musin::Logger &logger_;
  filesystem_t *fs_;

  bool format_filesystem(blockdevice_t *flash);
};

} // namespace musin::filesystem

#endif /* end of include guard: FILESYSTEM_H_A1PWKQIM */