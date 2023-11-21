#include "../../pins.h"
#include <Arduino.h>
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "led_pins.h"

#define PIN_AMP_MUTE	       GPIO_AD_11
#define PIN_HP_ENABLE	       GPIO_AD_11

#define PIN_SYN_MUX_IO       GPIO_AD_14
#define PIN_BRN_MUX_IO       GPIO_AD_02

#define PIN_SYN_ADDR0        GPIO_SD_00
#define PIN_SYN_ADDR1        GPIO_SD_01
#define PIN_SYN_ADDR2        GPIO_SD_02

#define BENCHMARK_PIN        GPIO_SD_13

// Channel 0 of BRN_MUX is connected to a resistor that goes nowhere
#define UNCONNECTED_ANALOG   1

static int muxAnalogRead(const uint8_t channel, const uint8_t mux_pin) {
  pinMode(mux_pin, INPUT_DISABLE);
  digitalWrite(PIN_SYN_ADDR0, bitRead(channel, 0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel, 1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel, 2));
  
  delayMicroseconds(50);
  return (analogRead(mux_pin));
}

static uint8_t muxDigitalRead(const uint8_t channel, const uint8_t mux_pin, PinMode mode) {
  pinMode(mux_pin, mode);

  digitalWrite(PIN_SYN_ADDR0, bitRead(channel, 0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel, 1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel, 2));
  delayMicroseconds(40);
  
  return digitalRead(mux_pin);
}

uint32_t potRead(const Pot pot) {
  switch (pot) {
    case FILTER_RES_POT:
      return muxAnalogRead(0, PIN_SYN_MUX_IO);
    case TEMPO_POT:
      return (1023 - muxAnalogRead(4, PIN_BRN_MUX_IO));
    case GATE_POT:
      return (1023 - muxAnalogRead(6, PIN_BRN_MUX_IO));
    case AMP_POT:
      return muxAnalogRead(3, PIN_SYN_MUX_IO);
    case FILTER_FREQ_POT:
      return muxAnalogRead(5, PIN_SYN_MUX_IO);
    case OSC_PW_POT:
      return muxAnalogRead(7, PIN_SYN_MUX_IO);
    case OSC_DETUNE_POT:
      return muxAnalogRead(6, PIN_SYN_MUX_IO);
    case AMP_ENV_POT:
      return muxAnalogRead(1, PIN_SYN_MUX_IO);
    default:
      return 500;
  }
}

bool pinRead(const Pin pin) {
  switch (pin) {
    case ACCENT_PIN:
      return !(muxDigitalRead(5, PIN_BRN_MUX_IO, INPUT));
    case GLIDE_PIN:
      return muxDigitalRead(4, PIN_SYN_MUX_IO, INPUT_PULLUP) == 0;
    case DELAY_PIN:
      return muxDigitalRead(2, PIN_SYN_MUX_IO, INPUT_PULLUP) != 0;
    case BITC_PIN:
      return !(muxDigitalRead(7, PIN_BRN_MUX_IO, INPUT));
    case HP_DETECT_PIN:
      return (muxDigitalRead(3, PIN_BRN_MUX_IO, INPUT));
    case SYNC_DETECT_PIN:
      // The pulldown on the SYNC_DETECT pin is too weak
      // So let's do an analogRead and compare it to 70%
      // of fullscale
      return !(muxAnalogRead(2, PIN_BRN_MUX_IO) < 700); 
    default:
      return false;
  }
}

void pins_init() {
  pinMode(PIN_HP_ENABLE, OUTPUT);
  pinMode(PIN_AMP_MUTE, OUTPUT);

  pinMode(PIN_SYN_ADDR0, OUTPUT);
  pinMode(PIN_SYN_ADDR1, OUTPUT);
  pinMode(PIN_SYN_ADDR2, OUTPUT);

  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);

  // These pins are available as benchmarking pins
  pinMode(GPIO_AD_03, OUTPUT);
  pinMode(GPIO_SD_13, OUTPUT);

  randomSeed(analogRead(GPIO_AD_03));
}

bool headphone_jack_detected() {
  return pinRead(HP_DETECT_PIN);
}