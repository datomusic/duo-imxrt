#include "Arduino.h"
#include "board_init.h"
#include "leds.h"
#include "pin_mux.h"

using LEDs::Pixel;

int main(void) {
  board_init();
  LEDs::init();

  const int PIXEL_COUNT = 19;
  Pixel pixels[PIXEL_COUNT];

  int led_index = 0;
  int color = 0;

  bool fast_speed = false;

  while (true) {
    if (++led_index >= PIXEL_COUNT) {
      led_index = 0;
      fast_speed = !fast_speed;
    }

    for (int i = 0; i < PIXEL_COUNT; ++i) {
      pixels[i] = Pixel{0, 0, 0};
    }

    switch (color) {
      case 0:
        pixels[led_index].r = 255;
        break;
      case 1:
        pixels[led_index].g = 255;
        break;
      case 2:
        pixels[led_index].b = 255;
        break;
    }

    if (++color > 2) {
      color = 0;
    }

    LEDs::show(pixels, PIXEL_COUNT);

    if (fast_speed) {
      delayMicroseconds(10000);
    } else {
      delayMicroseconds(100000);
    }
  }
}
