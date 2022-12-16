#include "bs814a.h"

#include "Arduino.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#define BS814A_CLOCK_PERIOD_US 20
#define PIN_BS814A_CLOCK 3U
#define PIN_BS814A_DATA 2U

namespace BS814A {

void init(){
    pinMode(PIN_BS814A_CLOCK, OUTPUT);
    pinMode(PIN_BS814A_DATA, INPUT);
}


/*
  Bit 0:3 are Key 1 to Key 4 state. 0 is touched, 1 is no touch
  Bit 4:6 is a checksum. The number of keys touched
  Bit 7 is a stop bit. It is always 1

  The BS814A pulls the data pin low to signal that a touch is detected
*/

#define clock_low() \
    digitalWrite(PIN_BS814A_CLOCK, LOW); \
    delayMicroseconds(BS814A_CLOCK_PERIOD_US);

#define clock_hi() \
    digitalWrite(PIN_BS814A_CLOCK, HIGH); \
    delayMicroseconds(BS814A_CLOCK_PERIOD_US);

void readRaw(const bool keys[KEY_COUNT]){
  if (digitalRead(PIN_BS814A_DATA) == 1) {
    return false;
  }

  uint8_t mask;

// LSBFIRST
  int active_keys = 0;

  for (mask = 0x01; mask; mask <<= 1) {
    if (digitalRead(PIN_BS814A_DATA)){
      ++counter;
      value |= mask;
    }

    clock_hi();
    clock_low();
  }

  int active_keys = 0;

  for (int key = 0; key < 4; ++key) {
    const bool active = !digitalRead(PIN_BS814A_DATA);
    keys[key] = active;

    if(active){
      ++active_keys;
    }

    clock_hi();
    clock_low();
  }

  uint8_t checksum_key_count = 0;
  uint8_t mask = 0x01;

  for(int i=0; i<3; ++i) {
    if (digitalRead(PIN_BS814A_DATA)){
      checksum_key_count |= mask;
    }

    clock_hi();
    clock_low();
    mask <<= 1;
  }


  // TODO: Do we need to read the stop bit?
  digitalRead(PIN_BS814A_DATA);
  clock_hi();
  clock_low();

  return checksum_key_count == active_keys;;
}
}
