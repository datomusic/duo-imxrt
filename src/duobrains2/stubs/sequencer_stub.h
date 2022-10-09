#include "midi_stub.h"

void note_on(uint8_t midi_note, uint8_t velocity, bool enabled) {}
void note_off() {}

int potRead(uint8_t num) {
  return 0;
}

const int SYNC_DETECT = 0;
const int SYNC_IN = 0;
const int TEMPO_POT = 0;

typedef int elapsedMillis;
