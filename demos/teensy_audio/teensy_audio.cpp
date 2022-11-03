#include "Arduino.h"
#include "Audio.h"
#include "board_init.h"
#include "clock_config.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "leds.h"
#include "pin_mux.h"

using LEDs::Pixel;

int main(void) {
  board_init();
  LEDs::init();

  pinMode(PIN_AMP_MUTE, OUTPUT);
  pinMode(PIN_HP_ENABLE, OUTPUT);
  pinMode(PIN_SYNC_OUT, OUTPUT);

  digitalWrite(PIN_AMP_MUTE, LOW);
  digitalWrite(PIN_HP_ENABLE, HIGH);

  Pixel pixels[0];

  AudioSynthWaveformSine sine1; // xy=174,384
  AudioOutputMQS mqs1;          // xy=356,386
  AudioConnection patchCord1(sine1, 0, mqs1, 0);
  AudioConnection patchCord2(sine1, 0, mqs1, 1);

  AudioMemory(64);
  AudioNoInterrupts();
  sine1.frequency(10);
  sine1.amplitude(0.5);
  AudioInterrupts();

  while (1) {
    pixels[0].r = pixels[0].r > 0 ? 0 : 200;
    LEDs::show(pixels, 1);

    delayMicroseconds(100000);
  }
}
