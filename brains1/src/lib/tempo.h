#ifndef TEMPO_H
#define TEMPO_H

class TempoHandler;

struct Tempo {
  void reset(){}
  void update_internal(TempoHandler &handler, const int potvalue);
};

#endif /* end of include guard: TEMPO_H */
