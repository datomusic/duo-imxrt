#include "../../pins.h"
#include <Arduino.h>
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#define PIN_SW_ACCENT        GPIO_00
#define PIN_SW_CRUSH         GPIO_02
#define PIN_HP_JACK_DETECT   GPIO_01
#define PIN_HP_ENABLE	       GPIO_AD_11

#define PIN_SYN_MUX_IO       GPIO_AD_14
#define PIN_SYN_ADDR0        GPIO_SD_00
#define PIN_SYN_ADDR1        GPIO_SD_01
#define PIN_SYN_ADDR2        GPIO_SD_02

#define PIN_ROW_1            GPIO_13
#define PIN_ROW_2            GPIO_12
#define PIN_ROW_3            GPIO_SD_04
#define PIN_ROW_4            GPIO_11

#define PIN_COL_1           GPIO_AD_09
#define PIN_COL_2           GPIO_AD_08
#define PIN_COL_3           GPIO_AD_07
#define PIN_COL_4           GPIO_AD_06
#define PIN_COL_5           GPIO_SD_13
#define PIN_COL_6           GPIO_AD_05

int muxAnalogRead(const uint8_t channel) {
  digitalWrite(PIN_SYN_ADDR0, bitRead(channel, 0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel, 1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel, 2));
  delayMicroseconds(50);

  return analogRead(PIN_SYN_MUX_IO);
}

uint8_t muxDigitalRead(const uint8_t channel) {
  pinMode(PIN_SYN_MUX_IO, INPUT_PULLUP);
  digitalWrite(PIN_SYN_ADDR0, bitRead(channel, 0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel, 1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel, 2));
  delayMicroseconds(50);
  // Wait a few microseconds for the selection to propagate.
  return digitalRead(PIN_SYN_MUX_IO);
}

uint32_t potRead(const Pot pot) {
  switch (pot) {
    case FILTER_RES_POT:
      return muxAnalogRead(0);
    case TEMPO_POT:
      return 1023 - analogRead(GPIO_AD_04);
    case GATE_POT:
      return 1023 - analogRead(GPIO_AD_03);
    case AMP_POT:
      return muxAnalogRead(3);
    case FILTER_FREQ_POT:
      return muxAnalogRead(5);
    case OSC_PW_POT:
      return muxAnalogRead(7);
    case OSC_DETUNE_POT:
      return muxAnalogRead(6);
    case AMP_ENV_POT:
      return muxAnalogRead(9);
    default:
      return 500;
  }
}

bool pinRead(const Pin pin) {
  switch (pin) {
    case ACCENT_PIN:
      return !digitalRead(PIN_SW_ACCENT);
    case GLIDE_PIN:
      return muxDigitalRead(4) == 0;
    case DELAY_PIN:
      return muxDigitalRead(2) != 0;
    case BITC_PIN:
      return !digitalRead(PIN_SW_CRUSH);
    default:
      return false;
  }
}

void pins_init() {
  pinMode(PIN_SW_ACCENT, INPUT_PULLUP);
  pinMode(PIN_SW_CRUSH, INPUT_PULLUP);

  pinMode(PIN_HP_ENABLE, OUTPUT);

  pinMode(PIN_HP_JACK_DETECT, INPUT);

  pinMode(PIN_SYN_ADDR0, OUTPUT);
  pinMode(PIN_SYN_ADDR1, OUTPUT);
  pinMode(PIN_SYN_ADDR2, OUTPUT);

  //   randomSeed(analogRead(UNCONNECTED_ANALOG));
}

bool headphone_jack_detected() {
  return pinRead(HP_DETECT_PIN);
}
