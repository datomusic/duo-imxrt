#include "Arduino.h"
#include "pin_mux.h"
#include "leds.h"

using LEDs::Pixel;

/* #define PIN        NEOPIXEL_PIN */
/* #define NUMPIXELS 8 */

/* Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); */
/* #define DELAYVAL 500 */

/* void setup() { */
/* #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000) */
/*   clock_prescale_set(clock_div_1); */
/* #endif */

/* } */

/* void loop() { */
/*   pixels.clear(); */

/*   for(int i=0; i<NUMPIXELS; i++) { */

/*     pixels.setPixelColor(i, pixels.Color(0, 150, 0)); */
/*     pixels.show(); */
/*     delay(DELAYVAL); */
/*   } */
/* } */

int main(void) {
  /* Board pin init */
  BOARD_ConfigMPU();
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();
  LEDs::init();
  init(); // Seeeduino init

  /* pixels.begin(); */

  int counter = 0;

  const int PIXEL_COUNT = 4;
  Pixel pixels[PIXEL_COUNT] = {Pixel{0, 0xFF, 0}, Pixel{0, 0, 0},
                               Pixel{0, 0xFF, 0}, Pixel{0xFF, 0x0, 0x0}};

  while (1) {
    if (++counter > 1000) {
      ++pixels[1].b;
      counter = 0;
      LEDs::show(pixels, PIXEL_COUNT);
    }

    delayMicroseconds(1);
  }
}
