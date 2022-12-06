#include "Arduino.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"

// TODO: Switch to Arduino pin mappings once those are available
#define BS814A_CLOCK_PERIOD_US 20
#define PIN_BS814A_CLOCK 3U
// #define BS814A_DATA_GPIO GPIO1
// #define BS814A_DATA_PIN 4U
// #define BS814A_DATA_MUX IOMUXC_GPIO_04_GPIOMUX_IO04
#define PIN_BS814A_DATA 2U

void BS814A_begin(void);
uint8_t BS814A_readRaw(void);

void BS814A_begin()
{
  // Clock pin configuration
  pinMode(PIN_BS814A_CLOCK, OUTPUT);
  pinMode(PIN_BS814A_DATA, INPUT);
}

/* 
  Bit 0:3 are Key 1 to Key 4 state. 0 is touched, 1 is no touch 
  Bit 4:6 is a checksum. The number of keys touched
  Bit 7 is a stop bit. It is always 1

  The BS814A pulls the data pin low to signal that a touch is detected
*/
uint8_t BS814A_readRaw() 
{
  if (digitalRead(PIN_BS814A_DATA) == 1)
  {
    return 0;
  }

  uint8_t value = 0;
  uint8_t mask;
  for (mask=0x01; mask; mask <<= 1) // LSBFIRST
  { 
    digitalWrite(PIN_BS814A_CLOCK, LOW);

    delayMicroseconds(BS814A_CLOCK_PERIOD_US);

    if (digitalRead(PIN_BS814A)) value |= mask;

    digitalWrite(PIN_BS814A_CLOCK, HIGH);

    delayMicroseconds(BS814A_CLOCK_PERIOD_US);
  }
  return value; 
}
