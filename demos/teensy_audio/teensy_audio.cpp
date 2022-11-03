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

  pinMode(PIN_SYNC_OUT, OUTPUT);
  Pixel pixels[0];

  AudioSynthWaveformSine sine1; // xy=174,384
  AudioOutputMQS mqs1;          // xy=356,386
  AudioConnection patchCord1(sine1, 0, mqs1, 0);
  AudioConnection patchCord2(sine1, 0, mqs1, 1);

  AudioNoInterrupts();
  AudioMemory(64);
  sine1.frequency(440);
  sine1.amplitude(0.1);
  AudioInterrupts();

  while (true) {
    if (AudioStream::update_scheduled) {
      AudioStream::update_all();
    }
    /* pixels[0].r = pixels[0].r > 0 ? 0 : 200; */
    /* LEDs::show(pixels, 1); */

    /* delayMicroseconds(100000); */
  }
}
