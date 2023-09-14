#include "leds.h"
#include "fsl_common_arm.h"
#include "fsl_flexio.h"
#include "fsl_gpio.h"
#include "teensy_audio_config.h"
#include <Arduino.h>
#include <Audio.h>

#include "fsl_flexio.h"
#include "lib/board_init.h"

static uint8_t _brightness = 255;
static CRGB correction = 0xFFD0D0;

static void init_flexio() {
  flexio_config_t fxioUserConfig;
  FLEXIO_GetDefaultConfig(&fxioUserConfig);
  FLEXIO_Init(FLEXIO1, &fxioUserConfig);
}

extern "C" void show_pixels_bytes(uint32_t size, const uint8_t *bytes);

extern "C" void no_interrupts() {
  DisableIRQ(DMA0_IRQn);
  __disable_irq();
}

extern "C" void yes_interrupts() {
  EnableIRQ(DMA0_IRQn);
  __enable_irq();
}

namespace LEDs {

static uint8_t bytes[300];

void init(void) { init_flexio(); }
void setBrightness(int brightness) { _brightness = brightness; }
void show(const Pixel *const pixels, uint8_t pixel_count) {
  bytes[0] = 200;
  bytes[1] = 0;
  bytes[2] = 0;
  bytes[3] = 200;
  bytes[4] = 0;
  bytes[5] = 100;
  bytes[6] = 200;
  bytes[7] = 0;
  bytes[8] = 100;
  show_pixels_bytes(9, bytes);
}
} // namespace LEDs
