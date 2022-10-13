#include "FastLED.h"
#include "leds.h"

void FastLED_::show() {
  LEDs::Pixel pixels[pixel_count];

  for (int i = 0; i < pixel_count; ++i) {
    pixels[i].r = crgbs[i].red;
    pixels[i].g = crgbs[i].green;
    pixels[i].b = crgbs[i].blue;
  }

  LEDs::show(pixels, pixel_count);
}
void FastLED_::clear() {
  // TODO
}

void FastLED_::setBrightness(int brightness) {}
void FastLED_::setCorrection(int correction) {}
