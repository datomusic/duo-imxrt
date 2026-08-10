# Building the Brains 1 firmware

_Revived 2026-08-10 on `brains1-revival`. Previous status: bit-rotted since the shared
code restructure in 683f9b7 (Feb 2024); see git history of this file for the breakage list._

## Building

```sh
cd brains1
make -j8            # produces brains1.hex / brains1.elf
make brains1.bin    # raw binary, used by `make dfu`
```

Toolchain: defaults to the newest Arm GNU Toolchain under
`/Applications/ArmGNUToolchain/` on macOS (system path on Linux). Override with
`make COMPILERPATH=/path/to/arm-none-eabi/bin/` (trailing slash required).
Verified with 14.2.rel1.

The MIDI library submodule must be checked out:
`git submodule update --init brains2/libraries/MIDI`

Flashing (unchanged): `make dfu` (dfu-util, 1fc9:8189), `make flash` (openocd),
`make upload` (Teensy Loader).

## What the revival changed

- **`brains1/src/lib/midi_wrapper.{h,cpp}`** (new): Brains 1 implementation of the
  `MIDI::` namespace API from `brains2/core/lib/midi_wrapper.h`, backed by the Arduino
  MIDI library 4.x on `Serial1` (DIN MIDI, pins 0/1) plus the Teensy 3 core `usbMIDI`.
  The old core has no usb sysex/start/stop handlers, so sysex is polled in
  `MIDI::read()` (type 7, length in `getData1()`) and transport messages are dispatched
  from the realtime callback.
- **`src/src.ino`**: ported from the pre-2024 shared API to the current one, following
  `brains2/apps/duo/main.cpp` — `Sequencer::Sequencer` object with callbacks instead of
  `step_enable[]`/`sequencer_advance()` globals, `MIDI::init(Callbacks{...})` instead of
  `PlatformMidi`/`usbMIDI` wiring, `MIDI::read(MIDI_CHANNEL)` in the loop,
  default-constructed `TempoHandler`. `synth.gateLength` now stores the raw pot value
  (the shared `sequencer_update()` does the 10–200 ms mapping).
- **`src/Leds.h`**: `led_update()` ported to the sequencer API (same rendering as
  brains2's `duo-firmware/src/Leds.h`), keeping the Brains 1 `analogWrite` panel LEDs.
- **`src/Power.h`**: `MIDI::sendControlChange`, `sequencer.release_all_notes()`.
- **`src/Synth.h`**: fixed `filter_resonance` (was an undeclared identifier).
- **`src/pins_duo_1.0.h`**: fixed invalid include guard (`PINS_DUO_1.0_H`).
- **`Makefile`**: `FIRMWARE_PATH` → `../shared/duo`; `COMPILERPATH` auto-detected and
  overridable; Arduino MIDI library included/compiled from
  `../brains2/libraries/MIDI/src`; `-std=gnu++14`.

## Caveats

- Builds clean (55 KB text, fits MK20DX256); **not yet tested on hardware**.
- ~186 warnings remain, nearly all from the vendored FastLED/Teensy core.
- `brains1` is still absent from CI (`.github/workflows/firmware-test.yml`).
