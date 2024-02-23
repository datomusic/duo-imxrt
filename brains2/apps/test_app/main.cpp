#include "app.h"
#include "lib/leds.h"

int main(void) {
  App::init(MIDI::Callbacks{});

  CRGB colors[2] = {CRGB::Green, CRGB::Orange};
  LEDs::Pixel button = colors[0];

  const byte midi_channel = 1;
  int counter = 0;
  int col_ind = 0;
  while (true) {
    if (counter++ > 1000) {
      counter = 0;
      col_ind = !col_ind;
      button = colors[col_ind];
    }

    App::update(midi_channel);
    LEDs::show(&button, 1);
  }

  return 0;
}
