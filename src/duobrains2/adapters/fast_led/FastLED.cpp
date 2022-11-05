#include "FastLED.h"
#include "lib/leds.h"

void FastLED_::show() {
  static_assert(sizeof(LEDs::Pixel) == sizeof(CRGB));

  LEDs::show((LEDs::Pixel *)raw_pixels, pixel_count);
  LEDs::Pixel pixels[pixel_count];

  for (int i = 0; i < pixel_count; ++i) {
    CRGB &raw = raw_pixels[i];
    LEDs::Pixel &pix = pixels[i];

    pix.r = raw.red;
    pix.g = raw.green;
    pix.b = raw.blue;
  }

  LEDs::show(pixels, pixel_count);
}

void FastLED_::clear() {
  memset(raw_pixels, 0, sizeof(raw_pixels));
}
