#include "Arduino.h"
#include "Audio.h"
#include "board_init.h"
#include "clock_config.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "pin_mux.h"

int main(void) {
  board_init();

  pinMode(PIN_SYNC_OUT, OUTPUT);

  AudioSynthWaveformSine sine1; // xy=174,384
  AudioOutputMQS mqs1;          // xy=356,386
  AudioConnection patchCord1(sine1, 0, mqs1, 0);
  AudioConnection patchCord2(sine1, 0, mqs1, 1);

  AudioNoInterrupts();
  AudioMemory(64);
  sine1.frequency(440);
  sine1.amplitude(0.1);
  AudioInterrupts();

  unsigned long sweep_update_time = millis() + 10;

  while (true) {

    if (millis() > sweep_update_time) {
      uint32_t speed_pot = analogRead(PIN_POT_2);
      uint32_t length_pot = analogRead(PIN_POT_1);
      AudioNoInterrupts();
      sine1.amplitude((4096 - length_pot) / 16384.0);
      sine1.frequency(4100 - speed_pot);
      AudioInterrupts();
    }
  }
}
