
#include "tempo.h"
#include "Arduino.h"
#include "TempoHandler.h"

void Tempo::update_internal(TempoHandler &handler) {
  int potvalue = potRead(TEMPO_POT);
  int tbpm = 240; // 2 x beats per minute

  if (potvalue < 128) {
    tbpm = map(potvalue, 0, 128, 60, 120);
  } else if (potvalue < 895) {
    tbpm = map(potvalue, 128, 895, 120, 400);
  } else {
    tbpm = map(potvalue, 895, 1023, 400, 1200);
  }
  _tempo_interval = 5000000 / tbpm;

  if ((micros() - _previous_clock_time) > _tempo_interval) {
    _previous_clock_time = micros();
    trigger();
  }
}
