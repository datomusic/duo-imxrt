#include "FastLED.h"
#include "lib/leds.h"

void FastLED_::show() {
   LEDs::show(raw_pixels, pixel_count);
}

void FastLED_::clear() {
  memset(raw_pixels, 0, sizeof(CRGB) * pixel_count);
}

void FastLED_::setBrightness(int brightness) {
  LEDs::setBrightness(brightness);
}
void FastLED_::setCorrection(int) {
  // TODO
}
