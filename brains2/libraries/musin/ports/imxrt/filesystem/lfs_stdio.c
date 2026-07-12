/*
 * Newlib syscalls over littlefs, so portable musin code can use plain
 * fopen/fread/fwrite/mkdir per the musin filesystem port contract.
 *
 * Overrides the weak stubs in core/syscall_stubs.c. File descriptors 0-2
 * (stdin/stdout/stderr) are accepted and discarded; descriptors 3 and up
 * map into a small static table of littlefs file handles.
 */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include "lfs.h"
#include "lfs_instance.h"

#define LFS_STDIO_FIRST_FD 3
#define LFS_STDIO_MAX_FILES 4

struct lfs_stdio_slot {
  bool used;
  lfs_file_t file;
  struct lfs_file_config config;
  uint8_t cache[256] __attribute__((aligned(4)));
};

static struct lfs_stdio_slot slots[LFS_STDIO_MAX_FILES];

static struct lfs_stdio_slot *slot_for_fd(int fd) {
  const int index = fd - LFS_STDIO_FIRST_FD;
  if (index < 0 || index >= LFS_STDIO_MAX_FILES || !slots[index].used) {
    return NULL;
  }
  return &slots[index];
}

static int lfs_err_to_errno(int err) {
  switch (err) {
  case LFS_ERR_NOENT:
    return ENOENT;
  case LFS_ERR_EXIST:
    return EEXIST;
  case LFS_ERR_ISDIR:
    return EISDIR;
  case LFS_ERR_NOTDIR:
    return ENOTDIR;
  case LFS_ERR_NOSPC:
    return ENOSPC;
  case LFS_ERR_NOMEM:
    return ENOMEM;
  case LFS_ERR_INVAL:
    return EINVAL;
  default:
    return EIO;
  }
}

int _open(const char *path, int flags, int mode) {
  (void)mode;
  lfs_t *lfs = musin_imxrt_lfs();
  if (lfs == NULL) {
    errno = ENODEV;
    return -1;
  }

  int index;
  for (index = 0; index < LFS_STDIO_MAX_FILES; index++) {
    if (!slots[index].used) {
      break;
    }
  }
  if (index == LFS_STDIO_MAX_FILES) {
    errno = ENFILE;
    return -1;
  }
  struct lfs_stdio_slot *slot = &slots[index];

  int lfs_flags = 0;
  switch (flags & O_ACCMODE) {
  case O_RDONLY:
    lfs_flags = LFS_O_RDONLY;
    break;
  case O_WRONLY:
    lfs_flags = LFS_O_WRONLY;
    break;
  case O_RDWR:
    lfs_flags = LFS_O_RDWR;
    break;
  }
  if (flags & O_CREAT) {
    lfs_flags |= LFS_O_CREAT;
  }
  if (flags & O_TRUNC) {
    lfs_flags |= LFS_O_TRUNC;
  }
  if (flags & O_APPEND) {
    lfs_flags |= LFS_O_APPEND;
  }
  if (flags & O_EXCL) {
    lfs_flags |= LFS_O_EXCL;
  }

  memset(&slot->config, 0, sizeof(slot->config));
  slot->config.buffer = slot->cache;

  const int err =
      lfs_file_opencfg(lfs, &slot->file, path, lfs_flags, &slot->config);
  if (err != LFS_ERR_OK) {
    errno = lfs_err_to_errno(err);
    return -1;
  }
  slot->used = true;
  return index + LFS_STDIO_FIRST_FD;
}

int _close(int fd) {
  if (fd < LFS_STDIO_FIRST_FD) {
    return 0;
  }
  struct lfs_stdio_slot *slot = slot_for_fd(fd);
  lfs_t *lfs = musin_imxrt_lfs();
  if (slot == NULL || lfs == NULL) {
    errno = EBADF;
    return -1;
  }
  const int err = lfs_file_close(lfs, &slot->file);
  slot->used = false;
  if (err != LFS_ERR_OK) {
    errno = lfs_err_to_errno(err);
    return -1;
  }
  return 0;
}

int _read(int fd, char *ptr, int len) {
  if (fd < LFS_STDIO_FIRST_FD) {
    return 0; /* no stdin */
  }
  struct lfs_stdio_slot *slot = slot_for_fd(fd);
  lfs_t *lfs = musin_imxrt_lfs();
  if (slot == NULL || lfs == NULL) {
    errno = EBADF;
    return -1;
  }
  const lfs_ssize_t nread = lfs_file_read(lfs, &slot->file, ptr, len);
  if (nread < 0) {
    errno = lfs_err_to_errno(nread);
    return -1;
  }
  return nread;
}

int _write(int fd, char *ptr, int len) {
  if (fd < LFS_STDIO_FIRST_FD) {
    return len; /* discard stdout/stderr */
  }
  struct lfs_stdio_slot *slot = slot_for_fd(fd);
  lfs_t *lfs = musin_imxrt_lfs();
  if (slot == NULL || lfs == NULL) {
    errno = EBADF;
    return -1;
  }
  const lfs_ssize_t written = lfs_file_write(lfs, &slot->file, ptr, len);
  if (written < 0) {
    errno = lfs_err_to_errno(written);
    return -1;
  }
  return written;
}

int _lseek(int fd, int offset, int whence) {
  struct lfs_stdio_slot *slot = slot_for_fd(fd);
  lfs_t *lfs = musin_imxrt_lfs();
  if (slot == NULL || lfs == NULL) {
    errno = EBADF;
    return -1;
  }
  const lfs_soff_t pos = lfs_file_seek(lfs, &slot->file, offset, whence);
  if (pos < 0) {
    errno = lfs_err_to_errno(pos);
    return -1;
  }
  return pos;
}

int _fstat(int fd, struct stat *st) {
  memset(st, 0, sizeof(*st));
  st->st_mode = (fd < LFS_STDIO_FIRST_FD) ? S_IFCHR : S_IFREG;
  return 0;
}

int _isatty(int fd) {
  return fd < LFS_STDIO_FIRST_FD;
}

int _unlink(const char *path) {
  lfs_t *lfs = musin_imxrt_lfs();
  if (lfs == NULL) {
    errno = ENODEV;
    return -1;
  }
  const int err = lfs_remove(lfs, path);
  if (err != LFS_ERR_OK) {
    errno = lfs_err_to_errno(err);
    return -1;
  }
  return 0;
}

int mkdir(const char *path, mode_t mode) {
  (void)mode;
  lfs_t *lfs = musin_imxrt_lfs();
  if (lfs == NULL) {
    errno = ENODEV;
    return -1;
  }
  const int err = lfs_mkdir(lfs, path);
  if (err != LFS_ERR_OK) {
    errno = lfs_err_to_errno(err);
    return -1;
  }
  return 0;
}
