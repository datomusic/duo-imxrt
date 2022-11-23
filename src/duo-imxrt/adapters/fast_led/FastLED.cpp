#include "FastLED.h"
#include "lib/leds.h"

bool FastLED_::show() {
  return LEDs::show(raw_pixels, pixel_count);
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
