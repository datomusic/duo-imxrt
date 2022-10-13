#include "Arduino.h"
#include "board_init.h"
#include "pinmap.h"
#include "pin_mux.h"
#include "leds.h"

#include "stubs/sequencer_stub.h"
#include "globals.h"
#include "duo1/Sequencer.h"

#include "stubs/leds_stub.h"
#include "duo1/Leds.h"

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
