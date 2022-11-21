#include "Arduino.h"
#include "pin_mux.h"
#include "board.h"

void board_init() {
  BOARD_RelocateVectorTableToRam();
  BOARD_ConfigMPU();
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();
  seeeduino_init(); // Seeeduino init
  
  analogReadResolution(10); // Set resolution to 10 bits, so from 0-1023
}
