#include "effect_bitcrusher.h"
#include "filter_variable.h"
#include "mixer.h"

namespace Output {

static AudioFilterStateVariable highpass;
static AudioFilterStateVariable lowpass;
static AudioAmplifier overdrive;
static AudioEffectBitcrusher crusher;
static AudioAmplifier amplifier;

static AudioConnection connections[] = {
    AudioConnection(amplifier, 0, lowpass, 0),
    AudioConnection(lowpass, 0, highpass, 0),
    AudioConnection(highpass, 2, crusher, 0),
    AudioConnection(crusher, 0, overdrive, 0)
};

AudioStream &get_input() {
  return amplifier;
}
AudioStream &get_output() {
  return overdrive;
}


void set_volume(float value) {
  amplifier.gain(value);
}

void set_highpass(const float value) {
  highpass.frequency(value * 10000.0f);
}

void set_lowpass(const float value) {
  lowpass.frequency((1 - value) * 10000.0f);
}

void set_bitcrusher(const float value) {
  crusher.sampleRate(value * 44100.0f);
}

void set_distortion(const float value) {
  overdrive.gain(1 + value);
}

void init(){
  crusher.bits(16);
  crusher.sampleRate(44100);
  set_highpass(0);
  set_lowpass(0);
  set_distortion(0);
}

} // namespace Output
