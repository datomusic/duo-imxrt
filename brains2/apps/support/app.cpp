#include "app.h"
#include "Arduino.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "lib/board_init.h"
#include "lib/leds.h"
#include "lib/pins.h"
#include "lib/usb/usb.h"
#include <USB-MIDI.h>

#define USB_STARTUP_DELAY_MS 280

#define SYSEX_DATO_ID 0x7D
#define SYSEX_DUO_ID 0x64
#define SYSEX_REBOOT_BOOTLOADER 0x0B

static void enter_bootloader() {
  const LEDs::Pixel center = CRGB::Teal;

  LEDs::clear();
  LEDs::show(&center, 1);
  BOARD_EnterROMBootloader();
}

static MIDI::SyxCallback *user_syx_callback = nullptr;
static void app_syx_callback(byte *const data, const unsigned length) {
  if (data[1] == SYSEX_DATO_ID && data[2] == SYSEX_DUO_ID &&
      data[3] == SYSEX_REBOOT_BOOTLOADER) {
    enter_bootloader();
  } else if (user_syx_callback) {
    user_syx_callback(data, length);
  }
}

static void init_dma() {
  DMAMUX_Init(DMAMUX);

  edma_config_t userConfig;
  EDMA_GetDefaultConfig(&userConfig);
  EDMA_Init(DMA0, &userConfig);
}

namespace App {

void init(MIDI::Callbacks midi_callbacks) {
  board_init();
  init_dma();
  LEDs::init();
  pins_init();

  delay(USB_STARTUP_DELAY_MS);
  DatoUSB::init();

  // This is needed to configure the UART peripheral correctly (used for MIDI).
  Serial.begin(31250U);

  user_syx_callback = midi_callbacks.sysex;
  midi_callbacks.sysex = app_syx_callback;
  MIDI::init(MIDI::Callbacks{midi_callbacks});
}

void update() {
  DatoUSB::background_update();
  MIDI::read();
}

void update(const byte midi_channel) {
  DatoUSB::background_update();
  MIDI::read(midi_channel);
}

} // namespace App
