#include <sys/stat.h>

// Weak so that ports with a real filesystem (e.g. littlefs via
// libraries/musin/ports/imxrt/filesystem/lfs_stdio.c) can override them.

int _getpid(void) {
  return -1;
}

int _kill(int pid, int sig) {
  return -1;
}

__attribute__((weak)) int _read(int file, char *ptr, int len) {
  return -1;
}

__attribute__((weak)) int _write(int file, char *ptr, int len) {
  return -1;
}

__attribute__((weak)) int _close(int file) {
  return -1;
}

__attribute__((weak)) int _fstat(int file, struct stat *st) {
  return 0;
}

__attribute__((weak)) int _isatty(int file) {
  return 0;
}

__attribute__((weak)) int _lseek(int file, int ptr, int dir) {
  return 0;
}
