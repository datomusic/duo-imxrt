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

namespace LEDs {

void init(void) { init_flexio(); }
void setBrightness(int brightness) { _brightness = brightness; }
void show(const Pixel *const pixels, const int pixel_count) {}
} // namespace LEDs
