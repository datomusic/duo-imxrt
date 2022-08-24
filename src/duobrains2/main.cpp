#include "Arduino.h"
#include "pin_mux.h"

int main(void) {
  /* Board pin init */
  BOARD_ConfigMPU();
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();
  init(); // Seeeduino init

  while (1) {
    delayMicroseconds(2020);
  }
}
