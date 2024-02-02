extern "C" {

int _getpid(void) {
  return -1;
}

int _kill(int pid, int sig) {
  return -1;
}

int _read(int file, char *ptr, int len) {
  return -1;
}

int _write(int file, char *ptr, int len) {
  return -1;
}

int _close(int file) {
  return -1;
}

int _fstat(int file, struct stat *st) {
  return 0;
}

int _isatty(int file) {
  return 0;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}
}
