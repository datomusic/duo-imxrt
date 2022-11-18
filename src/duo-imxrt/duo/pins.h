#pragma once

#define SEQ_1_2

#define LED_DATA 32
#define LED_CLK 25
#define SYNC_IN 14

enum DuoPin { GLIDE_PIN, DELAY_PIN, ACCENT_PIN, BITC_PIN };

enum Pot {
  FILTER_RES_POT,
  TEMPO_POT,
  GATE_POT,
  AMP_POT,
  FILTER_FREQ_POT,
  OSC_DETUNE_POT,
  OSC_PW_POT,
  AMP_ENV_POT
};

int potRead(const Pot pot);
bool pinRead(const DuoPin pin);
void pins_init();
