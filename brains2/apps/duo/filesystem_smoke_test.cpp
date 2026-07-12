#include "filesystem_smoke_test.h"

#include <cstdint>
#include <cstdio>

extern "C" {
#include "fsl_debug_console.h"
}

void filesystem_smoke_test(musin::filesystem::Filesystem &filesystem) {
  PRINTF("[fs] --- littlefs smoke test ---\r\n");

  // Bump a boot counter through plain stdio, proving the newlib wiring and
  // persistence across power cycles in one go.
  uint32_t boots = 0;
  FILE *file = fopen("/boot_count", "rb");
  if (file != nullptr) {
    if (fread(&boots, sizeof(boots), 1, file) != 1) {
      PRINTF("[fs] WARN: boot counter unreadable, resetting\r\n");
      boots = 0;
    }
    fclose(file);
  }
  boots++;

  file = fopen("/boot_count", "wb");
  if (file == nullptr) {
    PRINTF("[fs] FAIL: open boot counter for writing\r\n");
    return;
  }
  const bool written = fwrite(&boots, sizeof(boots), 1, file) == 1;
  fclose(file);
  if (!written) {
    PRINTF("[fs] FAIL: write boot counter\r\n");
    return;
  }

  // Read back through a fresh handle to verify what actually hit flash.
  uint32_t verify = 0;
  file = fopen("/boot_count", "rb");
  if (file == nullptr || fread(&verify, sizeof(verify), 1, file) != 1 ||
      verify != boots) {
    if (file != nullptr) {
      fclose(file);
    }
    PRINTF("[fs] FAIL: boot counter verify (got %lu, want %lu)\r\n",
           static_cast<unsigned long>(verify),
           static_cast<unsigned long>(boots));
    return;
  }
  fclose(file);

  const auto info = filesystem.get_storage_info();
  PRINTF("[fs] PASS: boot #%lu, %lu of %lu bytes free\r\n",
         static_cast<unsigned long>(boots),
         static_cast<unsigned long>(info.free_bytes),
         static_cast<unsigned long>(info.total_bytes));
}
