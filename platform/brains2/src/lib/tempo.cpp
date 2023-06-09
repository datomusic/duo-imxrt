#include "tempo.h"
#include <Arduino.h>

#define BPM_TO_MILLIS(bpm) (2500000 / bpm)

#include "firmware/TempoHandler.h"

void Tempo::init() { last_millis = millis(); }

void Tempo::update_internal(TempoHandler &handler, const int potvalue) {
  uint32_t scaled_millis_per_beat; // 2 x beats per minute

  if (potvalue < 128) {
    scaled_millis_per_beat =
        map(potvalue, 0, 128, BPM_TO_MILLIS(30), BPM_TO_MILLIS(60));
  } else if (potvalue < 895) {
    scaled_millis_per_beat =
        map(potvalue, 128, 895, BPM_TO_MILLIS(60), BPM_TO_MILLIS(200));
  } else {
    // For Toon: 603 BPM in gives 600 BPM out
    scaled_millis_per_beat =
        map(potvalue, 895, 1023, BPM_TO_MILLIS(200), BPM_TO_MILLIS(603));
  }

  const auto cur = millis();
  // Multiply by 100 as a scaling factor for millis_per_beat
  // and a factor of 10 to match original Duo
  accum += (cur - last_millis) * 1000;
  last_millis = cur;

  if (accum >= scaled_millis_per_beat) {
    accum = 0;
    handler._previous_clock_time = micros();
    handler.trigger();
  }
}
