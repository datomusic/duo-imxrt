#include "Arduino.h"
#include "lib/board_init.h"

extern "C" {
void rust_main();

void delay_mic(uint32_t mics) { delayMicroseconds(mics); };
}

int main(void) {
  board_init();

  rust_main();
  return 0;
}
