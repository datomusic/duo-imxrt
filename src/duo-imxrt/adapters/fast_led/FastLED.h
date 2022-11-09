#ifndef FASTLED_H_ACDOEHJU
#define FASTLED_H_ACDOEHJU
#include "./color_types.h"

struct FastLED_ {
  void show();
  void clear();
  void setBrightness(int);
  void setCorrection(int);

  // We ignore the template parameters, since our current
  // implementation is specific to a single chip+board currently.
  // We also implement add_leds wrongly, since we replace all pixels
  // rather than adding to them, which is also fine for current use-cases.
  // The current goal is simply to implement the FastLED interface well enough
  // for the Duo firmware to work.
  // For future work it makes more sense to properly port FastLED and use that.
  template <int _LedType, int _LedData, int _ColorOrder>
  void addLeds(CRGB *raw_pixels, const int count) {
    this->pixel_count = count;
    this->raw_pixels = raw_pixels;
  }

private:
  int pixel_count;
  CRGB *raw_pixels;
};

FastLED_ FastLED;

#endif /* end of include guard: FASTLED_H_ACDOEHJU */
