#ifndef SYNTH_PARAMS_H
#define SYNTH_PARAMS_H

// 10 bit representation of full scale parameters
typedef struct {
  int detune;
  int pulseWidth;
  int filter;
  int resonance;
  int release;
  int amplitude;
  int speed;
  bool glide;
  bool accent;
  bool delay;
  bool crush;
  int gateLength;
} synth_parameters;

#endif /* end of include guard: SYNTH_PARAMS_H */
