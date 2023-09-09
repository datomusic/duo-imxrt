#include "Arduino.h"
#include "lib/board_init.h"
#include "lib/leds.h"

using LEDs::Pixel;

const int PIXEL_COUNT = 19;
Pixel pixels[PIXEL_COUNT];

extern "C" {
void rust_main();

void show_pixels() { LEDs::show(pixels, PIXEL_COUNT); }
void delay_mic(uint32_t mics) { delayMicroseconds(mics); };
void set_pixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  if (index >= 0 && index < PIXEL_COUNT) {
    pixels[index] = Pixel{r, g, b};
  }
}
}

int main(void) {
  board_init();
  LEDs::init();

  rust_main();
  return 0;
}
