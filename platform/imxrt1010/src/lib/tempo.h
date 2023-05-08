#ifndef TEMPO_H
#define TEMPO_H

class TempoHandler;

struct Tempo {
  void init();
  void update_internal(TempoHandler &handler);

private:
  uint32_t accum = 0;
  uint32_t last_millis = 0;
};

#endif /* end of include guard: TEMPO_H */
