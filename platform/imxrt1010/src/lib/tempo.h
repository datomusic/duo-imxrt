#ifndef TEMPO_H
#define TEMPO_H

#include <cstdint>

class TempoHandler;

struct Tempo {
  void init();
  void update_internal(TempoHandler &handler, const int potvalue);

private:
  uint32_t accum = 0;
  uint32_t last_millis = 0;
};

#endif /* end of include guard: TEMPO_H */
