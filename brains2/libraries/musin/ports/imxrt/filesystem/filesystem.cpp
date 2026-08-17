// MIMXRT1011 port of the musin filesystem facade: littlefs on the tail of
// the XIP QSPI NOR flash, through the mflash driver. Reads go through the
// memory-mapped AHB window; erase/program run from ITCM with interrupts
// disabled (see mflash_drv.c).

#include "musin/filesystem/filesystem.h"
#include "musin/filesystem/storage_region.h"

#include "lfs.h"
#include "lfs_instance.h"
#include <cstring>

extern "C" {
#include "mflash_drv.h"
}

// The neutral header forward-declares this as an opaque type.
struct filesystem {
  lfs_t lfs;
  lfs_config cfg;
  bool mounted;
};

// Alias usable inside namespace musin::filesystem, where the bare name
// 'filesystem' resolves to the namespace instead of the struct above.
using PortFilesystem = filesystem;

namespace musin::filesystem {

namespace {

constexpr lfs_size_t READ_SIZE = 16;
constexpr lfs_size_t CACHE_SIZE = MFLASH_PAGE_SIZE;
constexpr lfs_size_t LOOKAHEAD_SIZE = 16;
constexpr int32_t BLOCK_CYCLES = 100;

PortFilesystem g_fs_instance;
uint32_t g_region_offset; // bytes from start of flash, sector aligned

alignas(4) uint8_t g_read_buffer[CACHE_SIZE];
alignas(4) uint8_t g_prog_buffer[CACHE_SIZE];
alignas(8) uint8_t g_lookahead_buffer[LOOKAHEAD_SIZE];

uint32_t block_phys_addr(lfs_block_t block, lfs_off_t off) {
  return g_region_offset + block * MFLASH_SECTOR_SIZE + off;
}

int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
             void *buffer, lfs_size_t size) {
  (void)c;
  const void *src = mflash_drv_phys2log(block_phys_addr(block, off), size);
  memcpy(buffer, src, size);
  return LFS_ERR_OK;
}

int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
             const void *buffer, lfs_size_t size) {
  (void)c;
  // littlefs calls with off and size multiples of prog_size, which equals
  // the flash page size, so every chunk is one aligned page program.
  const uint8_t *src = static_cast<const uint8_t *>(buffer);
  for (lfs_size_t chunk = 0; chunk < size; chunk += MFLASH_PAGE_SIZE) {
    const int32_t status = mflash_drv_page_program(
        block_phys_addr(block, off + chunk),
        (uint32_t *)(src + chunk)); // NOLINT: buffer is our aligned cache
    if (status != 0) {
      return LFS_ERR_IO;
    }
  }
  return LFS_ERR_OK;
}

int lfs_erase(const struct lfs_config *c, lfs_block_t block) {
  (void)c;
  const int32_t status = mflash_drv_sector_erase(block_phys_addr(block, 0));
  return (status == 0) ? LFS_ERR_OK : LFS_ERR_IO;
}

int lfs_sync(const struct lfs_config *c) {
  (void)c;
  return LFS_ERR_OK;
}

bool configure(PortFilesystem &fs, const StorageRegion &region) {
  if (region.offset % MFLASH_SECTOR_SIZE != 0 ||
      region.size % MFLASH_SECTOR_SIZE != 0 || region.size == 0) {
    return false;
  }
  g_region_offset = region.offset;

  fs.cfg = {};
  fs.cfg.read = lfs_read;
  fs.cfg.prog = lfs_prog;
  fs.cfg.erase = lfs_erase;
  fs.cfg.sync = lfs_sync;
  fs.cfg.read_size = READ_SIZE;
  fs.cfg.prog_size = MFLASH_PAGE_SIZE;
  fs.cfg.block_size = MFLASH_SECTOR_SIZE;
  fs.cfg.block_count = region.size / MFLASH_SECTOR_SIZE;
  fs.cfg.block_cycles = BLOCK_CYCLES;
  fs.cfg.cache_size = CACHE_SIZE;
  fs.cfg.lookahead_size = LOOKAHEAD_SIZE;
  fs.cfg.read_buffer = g_read_buffer;
  fs.cfg.prog_buffer = g_prog_buffer;
  fs.cfg.lookahead_buffer = g_lookahead_buffer;
  return true;
}

} // namespace

Filesystem::Filesystem(musin::Logger &logger) : logger_(logger), fs_(nullptr) {
}

bool Filesystem::init() {
  logger_.info("Initializing filesystem");

  const auto region = get_storage_region(logger_);
  if (!region) {
    return false;
  }
  logger_.info("Data region offset: ", region->offset);
  logger_.info("Data region size: ", region->size);

  if (mflash_drv_init() != 0) {
    logger_.error("mflash init failed");
    return false;
  }

  if (!configure(g_fs_instance, *region)) {
    logger_.error("Storage region is not sector aligned");
    return false;
  }
  fs_ = &g_fs_instance;

  int err = lfs_mount(&fs_->lfs, &fs_->cfg);
  if (err != LFS_ERR_OK) {
    logger_.warn("Initial mount failed, formatting", (int32_t)err);
    err = lfs_format(&fs_->lfs, &fs_->cfg);
    if (err != LFS_ERR_OK) {
      logger_.error("Format failed", (int32_t)err);
      return false;
    }
    err = lfs_mount(&fs_->lfs, &fs_->cfg);
    if (err != LFS_ERR_OK) {
      logger_.error("Mount after format failed", (int32_t)err);
      return false;
    }
  }

  fs_->mounted = true;
  logger_.info("Filesystem mounted");
  return true;
}

bool Filesystem::format() {
  logger_.info("Explicit format requested");

  if (fs_ == nullptr) {
    // Not initialized yet: set up the block device first.
    const auto region = get_storage_region(logger_);
    if (!region || mflash_drv_init() != 0 ||
        !configure(g_fs_instance, *region)) {
      return false;
    }
    fs_ = &g_fs_instance;
  }

  if (fs_->mounted) {
    lfs_unmount(&fs_->lfs);
    fs_->mounted = false;
  }

  int err = lfs_format(&fs_->lfs, &fs_->cfg);
  if (err != LFS_ERR_OK) {
    logger_.error("Format failed", (int32_t)err);
    return false;
  }
  err = lfs_mount(&fs_->lfs, &fs_->cfg);
  if (err != LFS_ERR_OK) {
    logger_.error("Mount after format failed", (int32_t)err);
    return false;
  }
  fs_->mounted = true;
  return true;
}

void Filesystem::list_files(const char *path) {
  if (fs_ == nullptr || !fs_->mounted) {
    logger_.warn("list_files: filesystem not mounted");
    return;
  }

  lfs_dir_t dir;
  if (lfs_dir_open(&fs_->lfs, &dir, path) != LFS_ERR_OK) {
    logger_.error("Error opening directory");
    return;
  }

  lfs_info info;
  while (lfs_dir_read(&fs_->lfs, &dir, &info) > 0) {
    if (info.type == LFS_TYPE_REG) {
      logger_.info(info.name, (uint32_t)info.size);
    } else {
      logger_.info(info.name);
    }
  }
  lfs_dir_close(&fs_->lfs, &dir);
}

StorageInfo Filesystem::get_storage_info() {
  StorageInfo info{0, 0};
  if (fs_ == nullptr || !fs_->mounted) {
    return info;
  }
  const lfs_ssize_t used_blocks = lfs_fs_size(&fs_->lfs);
  if (used_blocks < 0) {
    return info;
  }
  info.total_bytes = fs_->cfg.block_count * fs_->cfg.block_size;
  info.free_bytes =
      info.total_bytes - (uint32_t)used_blocks * fs_->cfg.block_size;
  return info;
}

} // namespace musin::filesystem

extern "C" lfs_t *musin_imxrt_lfs(void) {
  using namespace musin::filesystem;
  if (!g_fs_instance.mounted) {
    return nullptr;
  }
  return &g_fs_instance.lfs;
}
