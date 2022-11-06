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
  AMP_POT = 3,
  AMP_ENV_POT = 4,
  FILTER_FREQ_POT = 5,
  OSC_DETUNE_POT = 6,
  OSC_PW_POT = 7
};

int potRead(const Pot pot);
bool pinRead(const Pin pin);
void pins_init();
