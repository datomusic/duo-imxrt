#ifndef LEDS_H_U0M7G3TA
#define LEDS_H_U0M7G3TA

#include "fsl_common.h"
#include "pixeltypes.h"

// Control NeoPixel(WS2812) LEDs by bitbanging.

namespace LEDs {

typedef CRGB Pixel;

void init();
void show(const Pixel *const pixels, int count);

} // namespace LEDs

#endif /* end of include guard: LEDS_H_U0M7G3TA */
