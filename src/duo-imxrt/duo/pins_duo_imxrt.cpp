#include "pins_duo_imxrt.h"
#include <Arduino.h>

static int muxAnalogRead(const uint8_t channel) {
  // Any call to pinMode sets the port mux to GPIO mode.
  // We want to force it back to analog mode
  pinMode(PIN_SYN_MUX_IO, INPUT);

  digitalWrite(PIN_SYN_ADDR0, bitRead(channel, 0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel, 1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel, 2));
  delayMicroseconds(50);

  return analogRead(PIN_SYN_MUX_IO);
}

static uint8_t muxDigitalRead(const uint8_t channel) {
  pinMode(PIN_SYN_MUX_IO, INPUT_PULLUP);
  digitalWrite(PIN_SYN_ADDR0, bitRead(channel, 0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel, 1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel, 2));
  delayMicroseconds(50);
  // Wait a few microseconds for the selection to propagate.
  return digitalRead(PIN_SYN_MUX_IO);
}

int potRead(const Pot pot) {
  switch (pot) {
    case AMP_POT:
      return muxAnalogRead(AMP_POT);
    case TEMPO_POT:
      return 1023-analogRead(PIN_POT_2);
    case GATE_POT:
      return 1023-analogRead(PIN_POT_1);
    case FILTER_FREQ_POT:
      return muxAnalogRead(pot);
    case FILTER_RES_POT:
      return muxAnalogRead(pot);
    case OSC_PW_POT:
      return muxAnalogRead(pot);
    case OSC_DETUNE_POT:
      return muxAnalogRead(pot);
    case AMP_ENV_POT:
      return muxAnalogRead(pot);
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

  pinMode(SYNC_OUT_PIN, OUTPUT);

  pinMode(PIN_HP_ENABLE, OUTPUT);
  pinMode(PIN_SYNC_IN, INPUT);

  pinMode(PIN_HP_JACK_DETECT, INPUT);
  pinMode(PIN_SYNC_JACK_DETECT, INPUT);

  pinMode(PIN_SYN_ADDR0, OUTPUT);
  pinMode(PIN_SYN_ADDR1, OUTPUT);
  pinMode(PIN_SYN_ADDR2, OUTPUT);

  //   randomSeed(analogRead(UNCONNECTED_ANALOG));
}
