#include "Arduino.h"
#include "board_init.h"
#include "pin_mux.h"

#include "stubs/sequencer_stub.h"
#include "globals.h"
#include "Sequencer.h"

const int NUM_LEDS = 19;

#include "stubs/leds_stub.h"
#include "duoleds.h"
#include "leds.h"

int main(void) {
  board_init();
  LEDs::init();

  FastLED.addLeds<LED_TYPE, LED_DATA, COLOR_ORDER>(physical_leds, NUM_LEDS);

  int counter = 0;
for (int i = 0; i < NUM_LEDS; ++i) {
    physical_leds[i] = 0;
  }


  while (1) {
    if (counter >= NUM_LEDS) {
      counter = 0;
      for (int i = 0; i < NUM_LEDS; ++i) {
        physical_leds[i] = 0;
      }
    } else {
      physical_leds[counter].g = 140;
      counter = counter + 1;
    }

    FastLED.show();
    delayMicroseconds(400000);
  }
}
