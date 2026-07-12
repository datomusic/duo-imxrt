#ifndef DUO_FILESYSTEM_SMOKE_TEST_H
#define DUO_FILESYSTEM_SMOKE_TEST_H

#include "musin/filesystem/filesystem.h"

/**
 * @brief Boot-time littlefs smoke test.
 *
 * Bumps a boot counter file through POSIX stdio on the already-mounted
 * filesystem and reports over the debug console. Call after
 * duo::storage_init() and before Serial.begin(): the debug console shares
 * LPUART1 with DIN MIDI, so output is only readable in that window.
 *
 * Temporary scaffolding; remove once settings storage has proven itself.
 */
void filesystem_smoke_test(musin::filesystem::Filesystem &filesystem);

#endif
