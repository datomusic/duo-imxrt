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
    AudioConnection(Rompler::get_output(), 0, pop_suppressor, 0),
    AudioConnection(pop_suppressor, 0, speaker_preamp, 0),
    AudioConnection(speaker_preamp, 0, dac, 1)};

static void setup_audio() {
  AudioNoInterrupts();
  AudioMemory(192);
  // speaker_preamp.gain(2.0f);
  speaker_preamp.gain(0.1f);
  AudioInterrupts();
}

int main(void) {
  App::init(MIDI::Callbacks{});
  Audio::amp_disable();

  dac.begin();
  setup_audio();
  // Output::set_volume(0.2f);
  Audio::amp_enable();

  int counter = 0;
  while (true) {
    if (counter++ > 1000000) {
      Rompler::hihat.play();
      counter = 0;
      MIDI::sendNoteOn(0, 100, 1);
    }

    App::update();
  }

  return 0;
}
