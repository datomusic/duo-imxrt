#define ARDUINO 1

#include "AudioSampleSnare.h"
#include "app.h"
#include "board_audio_output.h"
#include "lib/audio.h"
#include "output.h"
#include "rompler.h"
#include <Audio.h>
#include <AudioStream.h>
#include <effect_fade.h>
#include <mixer.h>
#include <play_memory.h>

static BoardAudioOutput dac;
static AudioEffectFade pop_suppressor;
static AudioAmplifier speaker_preamp;

static AudioConnection connections[] = {
    AudioConnection(Rompler::get_output(), 0, Output::get_input(), 0),
    AudioConnection(Output::get_output(), 0, pop_suppressor, 0),
    AudioConnection(pop_suppressor, 0, speaker_preamp, 0),
    AudioConnection(speaker_preamp, 0, dac, 1)};

static void setup_audio() {
  AudioNoInterrupts();
  AudioMemory(192);
  Output::init();
  speaker_preamp.gain(2.0f);
  // speaker_preamp.gain(0.1f);
  // Output::set_distortion(100);
  AudioInterrupts();
}

static void handle_midi_on(byte channel, byte note, byte velocity) {
  switch (note % 4) {
  case 0:
    Rompler::kick.play();
    break;

  case 1:
    Rompler::snare.play();
    break;

  case 2:
    Rompler::hihat.play();
    break;
  case 3:
    Rompler::tom.play();
    break;
  }
}

static void handle_cc(byte channel, byte cc, byte midi_value) {
  const float amount = (float)midi_value / 127.0f;

  switch (cc) {
  case 7:
    Output::set_volume(amount);
    break;
  /*
  case 74:
    Output::set_filter_frequency(amount);
    break;
  */
  case 75:
    Output::set_highpass(amount);
    break;
  case 76:
    Output::set_lowpass(amount);
    break;
  case 77:
    Output::set_distortion(amount);
    break;

  case 78:
    Output::set_bitcrusher(amount);
    break;
  }
}

int main(void) {
  App::init(MIDI::Callbacks{.note_on = handle_midi_on, .cc = handle_cc});
  Audio::amp_disable();

  dac.begin();
  setup_audio();
  // Output::set_volume(0.2f);
  Audio::amp_enable();

  int counter = 0;
  while (true) {
    if (counter++ > 1000000) {
      counter = 0;
      MIDI::sendNoteOn(0, 100, 1);
    }

    App::update();
  }

  return 0;
}
