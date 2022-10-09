#include "Arduino.h"
#include "pin_mux.h"
#include "board_init.h"

#include "stubs/sequencer_stub.h"
#include "globals.h"
#include "Sequencer.h"

#include "stubs/leds_stub.h"
#include "Leds.h"


int main(void) {
  board_init();

  while (1) {
    delayMicroseconds(2020);
  }
}


