#ifndef LEDS_H_U0M7G3TA
#define LEDS_H_U0M7G3TA

#include "fsl_common.h"

// Control NeoPixel(WS2812) LEDs by bitbanging.

namespace LEDs {

// NeoPixel color order is GRB, and is represented by the order of fields in
// this struct.
struct Pixel {
  union {
    struct {
      union {
        uint8_t g;
        uint8_t green;
      };
      union {
        uint8_t r;
        uint8_t red;
      };
      union {
        uint8_t b;
        uint8_t blue;
      };
    };
    uint8_t raw[3];
  };
};

void init();
void show(const Pixel *const pixels, int count);

} // namespace LEDs

#endif /* end of include guard: LEDS_H_U0M7G3TA */
