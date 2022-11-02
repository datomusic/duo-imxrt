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

  bool update_responsibility = AudioStream::update_setup();
  assert(update_responsibility == true);

  AudioSynthWaveformSine sine1; // xy=174,384
  AudioOutputMQS mqs1;          // xy=356,386
  AudioConnection patchCord1(sine1, 0, mqs1, 0);
  AudioConnection patchCord2(sine1, 0, mqs1, 1);

  AudioNoInterrupts();
  AudioMemory(64);
  sine1.frequency(200);
  sine1.amplitude(0.5);
  AudioInterrupts();

  while (1) {
    if (AudioOutputMQS::needs_update) {
      AudioOutputMQS::needs_update = false;
      AudioStream::update_all();
    }
    /* pixels[0].r = pixels[0].r > 0 ? 0 : 200; */
    /* LEDs::show(pixels, 1); */

    /* delayMicroseconds(100000); */
  }
}
