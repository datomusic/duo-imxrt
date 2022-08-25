#include "Arduino.h"
#include "pin_mux.h"

#include "stubs/sequencer_stub.h"
#include "Sequencer.h"

#include "stubs/leds_stub.h"
#include "Leds.h"

static void init_board(){
  BOARD_ConfigMPU();
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();
  init(); // Seeeduino init
}

int main(void) {
  init_board();


  while (1) {
    delayMicroseconds(2020);
  }
}


