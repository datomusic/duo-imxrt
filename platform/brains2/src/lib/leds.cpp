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

namespace LEDs {

void init(void) { init_flexio(); }
void setBrightness(int brightness) { _brightness = brightness; }
void show(const Pixel *const pixels, uint8_t pixel_count) {
  const uint8_t MAX_PIXELS = 19;
  static uint8_t bytes[MAX_PIXELS];

  if (pixel_count > MAX_PIXELS) {
    pixel_count = MAX_PIXELS;
  }

  for (int i = 0; i < pixel_count; ++i) {
    const uint8_t ind = i * 3;
    bytes[ind] = pixels[i].r;
    bytes[ind + 1] = pixels[i].g;
    bytes[ind + 2] = pixels[i].b;
  }

  show_pixels_bytes(pixel_count * 3, bytes);
}
} // namespace LEDs
