#include "Arduino.h"
#include "Audio.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "lib/audio.h"
#include "lib/board_init.h"
#include "lib/clock_config.h"
#include "lib/pin_mux.h"

#include "duo-firmware/src/effect_custom_envelope.h"
#include "output_mqs.h"
#include <analyze_peak.h>
#include <effect_bitcrusher.h>
#include <effect_delay.h>
#include <effect_fade.h>
#include <effect_multiply.h>
#include <filter_variable.h>
#include <mixer.h>
#include <synth_dc.h>
#include <synth_pinknoise.h>
#include <synth_simple_drum.h>
#include <synth_waveform.h>
#include <synth_whitenoise.h>

// GUItool: begin automatically generated code
AudioSynthWaveform osc_pulse;        // xy=78.10000610351562,97
AudioSynthWaveform osc_saw;          // xy=79.10000610351562,44
AudioSynthWaveformDc dc1;            // xy=88.10000610351562,149
AudioEffectCustomEnvelope envelope2; // xy=227.10000610351562,149
AudioMixer4 mixer1;                  // xy=255.10000610351562,83
AudioFilterStateVariable filter1;    // xy=403.1000061035156,91
AudioEffectCustomEnvelope envelope1; // xy=560.1000061035156,81
AudioAnalyzePeak peak1;              // xy=705.1000061035156,37
AudioEffectDelay delay1;             // xy=712.0999755859375,174.10000610351562
AudioEffectBitcrusher bitcrusher1;   // xy=718.1000061035156,81
AudioMixer4 mixer_delay;             // xy=728.0999755859375,279.1000061035156
AudioMixer4 mixer_output;            // xy=861.1000061035156,100
AudioEffectFade pop_suppressor;      // xy=1062,292
AudioAnalyzePeak peak2;              // xy=987.1000061035156,151
AudioSynthNoiseWhite hat_noise1;
AudioSynthSimpleDrum kick_drum1;
AudioEffectCustomEnvelope hat_envelope1;
AudioSynthSimpleDrum hat_snappy;
AudioFilterStateVariable hat_filter_bp;
AudioFilterStateVariable hat_filter_hp;
AudioMixer4 hat_mixer;
AudioConnection patchCord1(osc_pulse, 0, mixer1, 1);
AudioConnection patchCord2(osc_saw, 0, mixer1, 0);
AudioConnection patchCord3(dc1, envelope2);
AudioConnection patchCord4(envelope2, 0, filter1, 1);
AudioConnection patchCord5(mixer1, 0, filter1, 0);
AudioConnection patchCord6(filter1, 0, envelope1, 0);
AudioConnection patchCord7(envelope1, peak1);
AudioConnection patchCord8(envelope1, bitcrusher1);
AudioConnection patchCord9(delay1, 0, mixer_output, 1);
AudioConnection patchCord10(delay1, 0, mixer_delay, 1);
AudioConnection patchCord11(bitcrusher1, 0, mixer_output, 0);
AudioConnection patchCord12(bitcrusher1, 0, mixer_delay, 0);
AudioConnection patchCord13(mixer_delay, delay1);
AudioConnection patchCord20(hat_noise1, hat_envelope1);
AudioConnection patchCord21(hat_envelope1, 0, hat_filter_hp, 0);
AudioConnection patchCord22(hat_filter_hp, 2, hat_filter_bp, 0);
AudioConnection patchCord23(hat_filter_bp, 1, hat_mixer, 0);
AudioConnection patchCord24(hat_snappy, 0, hat_mixer, 1);
AudioConnection patchCord25(hat_mixer, 0, mixer_delay, 3);
AudioConnection patchCord26(kick_drum1, 0, mixer_output, 2);
AudioConnection patchCord27(hat_mixer, 0, mixer_output, 3);
AudioConnection patchCord14(mixer_output, pop_suppressor);
AudioConnection patchCord15(mixer_output, peak2);
// GUItool: end automatically generated code

void synth_init() {
  // Oscillators
  osc_saw.begin(0.4, 110, WAVEFORM_SAWTOOTH);
  osc_pulse.pulseWidth(0.5);
  osc_pulse.begin(0.4, 220, WAVEFORM_PULSE);

  // Mixer mixes the oscillators - don't add up to more than 0.8 or the output
  // will clip
  mixer1.gain(0, 0.4); // OSC1
  mixer1.gain(1, 0.4); // OSC2

  // Filter
  filter1.resonance(0.7); // range 0.7-5.0
  filter1.frequency(400);
  filter1.octaveControl(4);

  // Amp envelope
  envelope1.attack(2);
  envelope1.decay(0);
  envelope1.sustain(1.0);
  envelope1.release(400);

  // Filter envelope
  dc1.amplitude(1.0); // Filter env needs an input signal
  envelope2.attack(15);
  envelope2.decay(0);
  envelope2.sustain(1.0);
  envelope2.release(300);

  // Bitcrusher - set to audio samplerate and max bitrate
  bitcrusher1.bits(16);
  bitcrusher1.sampleRate(44100);

  delay1.delay(0, 440);     // Delay time
  mixer_delay.gain(0, 0.0); // Delay input
  mixer_delay.gain(1, 0.4); // Delay feedback

#define MAIN_GAIN 0.8
#define DELAY_GAIN 0.6
#define KICK_GAIN 0.8
#define HAT_GAIN 0.9

  mixer_output.gain(0, MAIN_GAIN);
  mixer_output.gain(1, DELAY_GAIN); // Delay output
  mixer_output.gain(2, KICK_GAIN);  // Kick output
  mixer_output.gain(3, HAT_GAIN);   // Hat output
}

void set_amplitude(float amp) {
  osc_saw.amplitude(amp);
  osc_pulse.amplitude(amp);
}
void set_freq(float freq) {
  osc_saw.frequency(freq);
  osc_pulse.frequency(freq);
}

int main(void) {
  board_init();
  Audio::amp_enable();

  pinMode(PIN_SYNC_OUT, OUTPUT);

  AudioNoInterrupts();
  AudioMemory(128);

  AudioOutputMQS output;
  AudioConnection conn(pop_suppressor, 0, output, 1);

  synth_init();

  mixer_delay.gain(0, 0.5); // Delay input
  mixer_delay.gain(3, 0.4); // Hat delay input
  envelope1.release(10);

  AudioInterrupts();

  int counter = 0;
  bool toggle = false;

  while (true) {
    if (++counter > 50000) {
      counter = 0;
      if (toggle) {
        envelope1.noteOn();

      } else {
        envelope1.noteOff();
      }
      toggle = !toggle;
    }

    const uint32_t speed_pot = analogRead(PIN_POT_2);
    const uint32_t length_pot = analogRead(PIN_POT_1);

    AudioNoInterrupts();
    set_amplitude((4096 - length_pot) / 16384.0);
    set_freq(4100 - speed_pot);
    AudioInterrupts();
  }
}
