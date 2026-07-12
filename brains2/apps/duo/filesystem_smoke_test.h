#ifndef DUO_FILESYSTEM_SMOKE_TEST_H
#define DUO_FILESYSTEM_SMOKE_TEST_H

/**
 * @brief Boot-time littlefs smoke test.
 *
 * Mounts (formatting on first boot), bumps a boot counter file through
 * POSIX stdio, and reports over the debug console. Call after board_init()
 * and before Serial.begin(): the debug console shares LPUART1 with DIN
 * MIDI, so output is only readable in that window.
 *
 * Temporary scaffolding until settings storage lands; remove afterwards.
 */
void filesystem_smoke_test();

#endif
