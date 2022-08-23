#ifndef LEDS_H_U0M7G3TA
#define LEDS_H_U0M7G3TA

#include "fsl_common.h"

// Control NeoPixel(WS2812) leds by bitbanging.

namespace LEDs {
// NeoPixel color order is GRB, and the order of fields in this struct matters.
struct Pixel {
  uint8_t g;
  uint8_t r;
  uint8_t b;
};

void init(void);
void show(Pixel const *pixels, int count);

} // namespace LEDs

#endif /* end of include guard: LEDS_H_U0M7G3TA */
