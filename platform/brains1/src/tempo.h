#ifndef TEMPO_H
#define TEMPO_H

class TempoHandler;

struct Tempo {
  void init(){}
  void update_internal(TempoHandler &handler);
};

#endif /* end of include guard: TEMPO_H */