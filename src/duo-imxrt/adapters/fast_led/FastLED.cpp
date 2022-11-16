#include "FastLED.h"
#include "lib/leds.h"

void FastLED_::show() {
  LEDs::show(raw_pixels, pixel_count);
}

void FastLED_::clear() {
  memset(raw_pixels, 0, sizeof(raw_pixels));
}

void FastLED_::setBrightness(int){
  // TODO
}
void FastLED_::setCorrection(int){
  // TODO
}
