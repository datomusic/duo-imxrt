#ifndef FASTLED_H_ACDOEHJU
#define FASTLED_H_ACDOEHJU
#include "../../stubs/fastled_stub.h"

struct FastLED_ {
  void show();
  void clear();
  void setBrightness(int);
  void setCorrection(int);

  template <int A, int B, int C>
  void addLeds(CRGB *raw_pixels, const int count) {
    pixel_count = count;
    crgbs = raw_pixels;
  }

private:
  int pixel_count;
  CRGB *crgbs;
};

FastLED_ FastLED;

#endif /* end of include guard: FASTLED_H_ACDOEHJU */
