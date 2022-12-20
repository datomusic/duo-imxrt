#include "bs814a.h"

#include "Arduino.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#define BS814A_CLOCK_PERIOD_US 20
#define PIN_BS814A_CLOCK 3U
#define PIN_BS814A_DATA 2U

#define clock_low()                                                            \
  digitalWrite(PIN_BS814A_CLOCK, LOW);                                         \
  delayMicroseconds(BS814A_CLOCK_PERIOD_US);

#define clock_hi()                                                             \
  digitalWrite(PIN_BS814A_CLOCK, HIGH);                                        \
  delayMicroseconds(BS814A_CLOCK_PERIOD_US);


namespace BS814A {

void init() {
  pinMode(PIN_BS814A_CLOCK, OUTPUT);
  pinMode(PIN_BS814A_DATA, INPUT);
  clock_low();
  clock_hi();
}

/*
  Bit 0:3 are Key 1 to Key 4 state. 0 is touched, 1 is no touch
  Bit 4:6 is a checksum. The number of keys touched
  Bit 7 is a stop bit. It is always 1

  The BS814A pulls the data pin low to signal that a touch is detected
*/

bool readRaw(TouchState *state) {
  if (digitalRead(PIN_BS814A_DATA) == 1) {
    return false;
  }

  int active_keys = 0;
  uint8_t mask = 0x01;

  (*state) = 0;

  for (int key = 0; key < KEY_COUNT; ++key) {
    clock_low();

    if (!digitalRead(PIN_BS814A_DATA)) {
      (*state) |= mask;
      ++active_keys;
    }

    clock_hi();
    mask <<= 1;
  }

  uint8_t checksum_key_count = 0;
  mask = 0x01;

  for (int i = 0; i < 3; ++i) {
    clock_low();
    if (digitalRead(PIN_BS814A_DATA)) {
      checksum_key_count |= mask;
    }

    clock_hi();
    mask <<= 1;
  }

  clock_low();

  // Read the stop bit
  digitalRead(PIN_BS814A_DATA);
  clock_hi();

  return checksum_key_count == active_keys;
}
} // namespace BS814A
