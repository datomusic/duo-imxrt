#pragma once
/*
 * pinmap.h maps the physical IO pins to software functions
 * the hardware pin mappings can be found in variant.h
 */
#define SEQ_1_2
#define MUX_IO PIN_SYN_MUX_IO
// DAC_OUT
#define TOUCH1 16
#define TOUCH2 17
#define TOUCH3 19
#define TOUCH4 18
#define LED_DATA 32
#define LED_CLK 25
#define JACK_DETECT 26
#define SYNC_DETECT 2
#define SYNC_IN 14
#define LED_1 20
#define LED_2 21
#define LED_3 5
#define SW1 8
#define SW2 30

// One more LED than the physical number of leds for loopback testing
const int NUM_LEDS = 19 + 1;
const int led_order[NUM_LEDS] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

// Multiplexer channels.

enum Pin {
  SLIDE_PIN,
  DELAY_PIN,
  ACCENT_PIN,
  BITC_PIN

};

enum Pot {
  AMP_POT,
  AMP_ENV_POT,
  FILTER_RES_POT,
  FILTER_FREQ_POT,
  OSC_DETUNE_POT,
  OSC_PW_POT,
  TEMPO_POT,
  GATE_POT
};

/*
int muxAnalogRead(uint8_t channel) {
  // Any call to pinMode sets the port mux to GPIO mode.
  // We want to force it back to analog mode
  // Might be equivalent to pinMode(MUX_IO, INPUT_DISABLE);
  //  volatile uint32_t *config;
  //  config = portConfigRegister(PIN_SYN_MUX_IO);
  //  *config = PORT_PCR_MUX(0);

  digitalWrite(PIN_SYN_ADDR0, bitRead(channel, 0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel, 1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel, 2));
  // do we need to wait a few microseconds?
  delayMicroseconds(50);
  return analogRead(PIN_SYN_MUX_IO);
}

uint8_t muxDigitalRead(const Pin channel) {
  pinMode(PIN_SYN_MUX_IO, INPUT_PULLUP);
  digitalWrite(PIN_SYN_ADDR0, bitRead(channel, 0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel, 1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel, 2));
  delayMicroseconds(50);
  // Wait a few microseconds for the selection to propagate.
  uint8_t p = digitalRead(PIN_SYN_MUX_IO);
  return p;
}
*/

int potRead(const Pot pot) {
  switch (pot) {
    case TEMPO_POT:
      return 1023 - analogRead(PIN_POT_1);
    case GATE_POT:
      return 1023 - analogRead(PIN_POT_2);
    case FILTER_FREQ_POT:
      return 500;
    case AMP_ENV_POT:
      return 500;
    case OSC_PW_POT:
      return 500;
    case AMP_POT:
      return 500;
    case OSC_DETUNE_POT:
      return 500;
    case FILTER_RES_POT:
      return 500;
    default:
      return 500;
  }
}

bool pinRead(const Pin pin) {
  switch (pin) {
    case SLIDE_PIN:
      return false;
    case DELAY_PIN:
      return false;
    case BITC_PIN:
      return false;
    case ACCENT_PIN:
      return false;
    default:
      return false;
  }
}
