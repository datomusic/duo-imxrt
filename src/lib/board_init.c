#include "Arduino.h"
#include "pin_mux.h"
#include "board.h"

void board_init() {
  BOARD_RelocateVectorTableToRam();
  BOARD_ConfigMPU();
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();
  init(); // Seeeduino init
}
