#ifndef LEDS_H_U0M7G3TA
#define LEDS_H_U0M7G3TA

#include "fsl_common.h"
#include "pixeltypes.h"

// Control NeoPixel(WS2812) LEDs by bitbanging.

namespace LEDs {

typedef CRGB Pixel;

void init();
void clear();
void show(const Pixel *const pixels, uint16_t count);
void setBrightness(int brightness);
uint8_t _brightness = 255;
CRGB correction = 0xFFD0D0;

} // namespace LEDs

#endif /* end of include guard: LEDS_H_U0M7G3TA */
