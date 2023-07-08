#include "lib/tempo.h"
#include <Arduino.h>

#define BPM_TO_MILLIS(bpm) (2500000 / bpm)

#include "firmware/TempoHandler.h"

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

  const uint32_t cur = millis();
  // Multiply by 100 as a scaling factor for millis_per_beat
  // and a factor of 10 to match original Duo
  // Check if last_millis overflowed since last time.
  if (cur >= last_millis) {
    accum += (cur - last_millis) * 1000;
  }

  last_millis = cur;

  while (accum >= scaled_millis_per_beat) {
    accum -= scaled_millis_per_beat;
    handler._previous_clock_time = micros();
    handler.trigger();
  }
}

void Tempo::reset() {
  accum = 0;
  last_millis = millis();
}
