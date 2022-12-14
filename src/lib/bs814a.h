#include "Arduino.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#define BS814A_CLOCK_PERIOD_US 20
#define PIN_BS814A_CLOCK 3U
#define PIN_BS814A_DATA 2U

struct TouchState {
  bool key1;
  bool key2;
  bool key3;
  bool key4;
};

void BS814A_begin(void);
TouchState BS814A_readRaw(void);

void BS814A_begin() {
  pinMode(PIN_BS814A_CLOCK, OUTPUT);
  pinMode(PIN_BS814A_DATA, INPUT);
}

/*
  Bit 0:3 are Key 1 to Key 4 state. 0 is touched, 1 is no touch
  Bit 4:6 is a checksum. The number of keys touched
  Bit 7 is a stop bit. It is always 1

  The BS814A pulls the data pin low to signal that a touch is detected
*/

TouchState BS814A_readRaw() {
  TouchState s;
  s.key1 = false;
  s.key2 = false;
  s.key3 = false;
  s.key4 = false;

  if (digitalRead(PIN_BS814A_DATA) == 1) {
    return s;
  }

  uint8_t value = 0;
  uint8_t mask;
  for (mask = 0x01; mask; mask <<= 1) // LSBFIRST
  {
    digitalWrite(PIN_BS814A_CLOCK, LOW);

    delayMicroseconds(BS814A_CLOCK_PERIOD_US);

    if (digitalRead(PIN_BS814A_DATA))
      value |= mask;

    digitalWrite(PIN_BS814A_CLOCK, HIGH);

    delayMicroseconds(BS814A_CLOCK_PERIOD_US);
  }

  s.key1 = ((value & 0x01) == 0);
  s.key2 = ((value & (0x01 << 1)) == 0);
  s.key3 = ((value & (0x01 << 2)) == 0);
  s.key4 = ((value & (0x01 << 3)) == 0);

  return s;
}
