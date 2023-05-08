#ifndef TEMPO_H
#define TEMPO_H

bool tempo_tick(const int potvalue, const uint32_t prev_clock_time, uint32_t& tempo_interval) {
  int tbpm = 240; // 2 x beats per minute

  if (potvalue < 128) {
    tbpm = map(potvalue, 0, 128, 60, 120);
  } else if (potvalue < 895) {
    tbpm = map(potvalue, 128, 895, 120, 400);
  } else {
    tbpm = map(potvalue, 895, 1023, 400, 1200);
  }

  tempo_interval = 5000000 / tbpm;

  return (micros() - prev_clock_time) > tempo_interval;
}

#endif /* end of include guard: TEMPO_H */
