#pragma once
/*
 * pinmap.h maps the physical IO pins to software functions
 * the hardware pin mappings can be found in variant.h
 */
#define SEQ_1_2

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

// One more LED than the physical number of leds for loopback testing
const int NUM_LEDS = 19 + 1;
const int led_order[NUM_LEDS] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

// Multiplexer channels.

enum Pin { SLIDE_PIN = 1, DELAY_PIN = 2, ACCENT_PIN = 8, BITC_PIN = 30 };

enum Pot {
  FILTER_RES_POT = 0,
  TEMPO_POT = 1,
  GATE_POT = 2,
  OSC_DETUNE_POT = 3,
  AMP_ENV_POT = 4,
  FILTER_FREQ_POT = 5,
  AMP_POT = 6,
  OSC_PW_POT = 7
};

int muxAnalogRead(const uint8_t channel) {
  digitalWrite(PIN_SYN_ADDR0, bitRead(channel, 0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel, 1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel, 2));
  delayMicroseconds(50);

  // The rest of the firmware expected values between 0 and 1023.
  // Scaling it down by a factor of 4 seems to be roughly correct...
  int v = analogRead(PIN_SYN_MUX_IO) / 4;

  if (v > 1023) {
    return 1023;
  }else{
    return v;
  }
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

int potRead(const Pot pot) {
  switch (pot) {
    case AMP_POT:
      return muxAnalogRead(AMP_POT);
    case TEMPO_POT:
      return 1023 - analogRead(PIN_POT_1);
    case GATE_POT:
      return 1023 - analogRead(PIN_POT_2);
    case FILTER_FREQ_POT:
      return muxAnalogRead(pot);
    case FILTER_RES_POT:
      return muxAnalogRead(pot);
    case OSC_PW_POT:
      return muxAnalogRead(pot);
    case OSC_DETUNE_POT:
      return muxAnalogRead(pot);
    case AMP_ENV_POT:
      // TODO: This does not seem to work
      return muxAnalogRead(pot);
    default:
      return 500;
  }
}

bool pinRead(const Pin pin) {
  switch (pin) {
    case SLIDE_PIN:
      return false;
    case DELAY_PIN:
      return muxDigitalRead(pin) != 0;
    case BITC_PIN:
      return false;
    case ACCENT_PIN:
      return false;
    default:
      return false;
  }
}

void pins_init() {
  // pinMode(BITC_PIN, INPUT_PULLUP);
  // pinMode(ACCENT_PIN, INPUT_PULLUP);

  pinMode(SYNC_OUT_PIN, OUTPUT);
  // pinMode(AMP_ENABLE, OUTPUT);
  pinMode(PIN_HP_ENABLE, OUTPUT);
  pinMode(PIN_SYNC_IN, INPUT);

  pinMode(PIN_HP_JACK_DETECT, INPUT);
  pinMode(PIN_SYNC_JACK_DETECT, INPUT);

  pinMode(PIN_SYN_ADDR0, OUTPUT);
  pinMode(PIN_SYN_ADDR1, OUTPUT);
  pinMode(PIN_SYN_ADDR2, OUTPUT);

//   randomSeed(analogRead(UNCONNECTED_ANALOG));
}
