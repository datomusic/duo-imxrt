#include "AudioSampleSnare.h" // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "app.h"
#include "lib/audio.h"
#include <Audio.h>
#include <AudioStream.h>
#include <effect_fade.h>
#include <mixer.h>
#include <output_pt8211.h>
#include <play_memory.h>
#include <synth_waveform.h>

int main(void) {
  App::init(MIDI::Callbacks{});

  Audio::amp_disable();

  AudioPlayMemory sound0;
  AudioMixer4 mix; // two 4-channel mixers are needed in
  AudioAmplifier speaker_preamp;
  AudioOutputPT8211 dac;
  AudioEffectFade pop_suppressor;

  AudioSynthWaveform osc_saw; // xy=79.10000610351562,44

  // AudioConnection c1(sound0, 0, mix, 0);
  AudioConnection c1(osc_saw, 0, mix, 0);
  AudioConnection c2(mix, 0, pop_suppressor, 0);
  AudioConnection c3(pop_suppressor, 0, speaker_preamp, 0);
  AudioConnection c4(speaker_preamp, 0, dac, 1);

  dac.begin();
  AudioNoInterrupts();
  osc_saw.begin(0.4f, 400, WAVEFORM_BANDLIMIT_SAWTOOTH);
  speaker_preamp.gain(2.0f);
  AudioMemory(192); // 260 bytes per block, 2.9ms per block
  mix.gain(0, 0.2);
  AudioInterrupts();

  const byte midi_channel = 1;
  int counter = 0;
  int note = 0;

  Audio::amp_enable();

  while (true) {
    if (counter++ > 1000000) {

      sound0.play(AudioSampleSnare);
      counter = 0;
      note++;
      MIDI::sendNoteOn(note % 100, 100, 1);
    }

    App::update(midi_channel);
  }

  return 0;
}
